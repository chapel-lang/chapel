import os
import sys

# add jenkins config location to PATH 
sys.path.append(os.environ['CI_SITE_CONFIG'])

import ci_site_config
import argparse
import subprocess
import shlex
import common
import re
import shutil

def build_libfabric(libfab_install_path, mode):

        if (os.path.exists(libfab_install_path) != True):
            os.makedirs(libfab_install_path)  

        config_cmd = ['./configure', '--prefix={}'.format(libfab_install_path)]
        enable_prov_val = 'yes'

        if (mode == 'dbg'):
            config_cmd.append('--enable-debug')                     
        elif (mode == 'dl'):
            enable_prov_val='dl'
                   
        for prov in common.enabled_prov_list:
            config_cmd.append('--enable-{}={}'.format(prov, enable_prov_val))
        for prov in common.disabled_prov_list:
             config_cmd.append('--enable-{}=no'.format(prov))
       
        config_cmd.append('--with-psm2-src={}/opa-psm2-lib'.format(workspace))   
  
           
        common.run_command(['./autogen.sh'])
        common.run_command(shlex.split(" ".join(config_cmd)))
        common.run_command(['make','clean'])
        common.run_command(['make'])
        common.run_command(['make','install'])


def build_fabtests(libfab_install_path, mode):
       
    os.chdir('{}/fabtests'.format(workspace))
    if (mode == 'dbg'):   
        config_cmd = ['./configure', '--enable-debug', '--prefix={}' \
                      .format(libfab_install_path),'--with-libfabric={}' \
                      .format(libfab_install_path)] 
    else:
        config_cmd = ['./configure', '--prefix={}'.format(libfab_install_path),
                '--with-libfabric={}'.format(libfab_install_path)]


    common.run_command(['./autogen.sh'])
    common.run_command(config_cmd)
    common.run_command(['make','clean'])
    common.run_command(['make'])
    common.run_command(['make', 'install'])

def build_shmem(shmem_dir, libfab_install_path):

    shmem_tar = ci_site_config.shmem_tar
    if(os.path.exists(shmem_dir)):
        os.rmdir(shmem_dir)
    
    os.makedirs(shmem_dir)
    os.chdir(shmem_dir)
    
    os.makedirs('SOS')
    common.run_command(['tar', '-xf', shmem_tar, '-C', 'SOS', '--strip-components=1'])
    os.chdir('SOS')

    common.run_command(['./autogen.sh'])

    config_cmd = ['./configure', '--prefix={}'.format(shmem_dir), '--disable-fortran', \
                  '--enable-remote-virtual-addressing', '--disable-aslr-check', \
                  '--enable-pmi-simple', '--with-ofi={}'.format(libfab_install_path), \
                  'LDFLAGS=-fno-pie']

    common.run_command(config_cmd)
   
    common.run_command(['make','-j4'])
    common.run_command(['make', 'check', 'TESTS='])
    common.run_command(['make', 'install'])


def build_ISx(shmem_dir):
    
    oshcc = '{}/bin/oshcc'.format(shmem_dir)
    tmp_isx_src = '{}/ISx'.format(ci_site_config.shmem_root)
    shutil.copytree(tmp_isx_src, '{}/ISx'.format(shmem_dir)) 
    #os.chdir(shmem_dir)
    #git_cmd = ['git', 'clone', '--depth', '1', 'https://github.com/ParRes/ISx.git', 'ISx']
    
    #common.run_command(git_cmd) 
    os.chdir('{}/ISx/SHMEM'.format(shmem_dir))
    common.run_command(['make', 'CC={}'.format(oshcc), 'LDLIBS=-lm']) 
                  
    
def build_PRK(shmem_dir):
    
    oshcc = '{}/bin/oshcc'.format(shmem_dir)
    shmem_src = '{}/SOS'.format(shmem_dir)
    tmp_prk_src = '{}/PRK'.format(ci_site_config.shmem_root)
    shutil.copytree(tmp_prk_src, '{}/PRK'.format(shmem_dir))
    #os.chdir(shmem_dir)
    #git_cmd = ['git', 'clone', '--depth', ' 1', 'https://github.com/ParRes/Kernels.git', 'PRK']
    #common.run_command(git_cmd)
    os.chdir('{}/PRK'.format(shmem_dir))
    with open('common/make.defs','w') as f:
        f.write('SHMEMCC={} -std=c99\nSHMEMTOP={}\n'.format(oshcc,shmem_src))

    common.run_command(['make', 'allshmem'])

def build_uh(shmem_dir):
    oshcc_bin = "{}/bin".format(shmem_dir)
    os.environ["PATH"] += os.pathsep + oshcc_bin
    tmp_uh_src = '{}/tests-uh'.format(ci_site_config.shmem_root)
    shutil.copytree(tmp_uh_src, '{}/tests-uh'.format(shmem_dir))
    #os.chdir(shmem_dir) 
    #git_cmd = ['git', 'clone', '--depth', '1', 'https://github.com/openshmem-org/tests-uh.git', 'tests-uh'] 
    #common.run_command(git_cmd)
    os.chdir('{}/tests-uh'.format(shmem_dir))
    common.run_command(['make', '-j4', 'C_feature_tests'])
    

def build_mpi(mpi, mpisrc, mpi_install_path, libfab_install_path,  ofi_build_mode):
   
    build_mpi_path ="/mpibuilddir/{}-build-dir/{}/{}/{}".format(mpi, jobname, buildno, ofi_build_mode)
    if (os.path.exists(build_mpi_path) == False):
        os.makedirs(build_mpi_path)

    os.chdir(build_mpi_path)
    cmd = ["{}/configure".format(mpisrc),
                    "--disable-oshmem", "--prefix={}".format(mpi_install_path),
                    "--with-libfabric={}".format(libfab_install_path)]

    if (mpi == 'ompi'):
        cmd.append("--enable-mpi-fortran=no")
    elif (mpi == 'mpich'):
        cmd.append("--enable-fortran=no")
        cmd.append("--with-device=ch4:ofi")
        cmd.append("--enable-ch4-direct=netmod")
        
    configure_cmd = shlex.split(" ".join(cmd))
    common.run_command(configure_cmd)
    common.run_command(["make", "clean"])
    common.run_command(["make", "install", "-j32"])

def build_mpich_suite(mpi, mpi_install_path, libfab_install_path, ofi_build_mode):

    mpich_suite_build_path = "/mpibuilddir/mpich-suite-build-dir/{}/{}/{}/mpich" \
                             .format(jobname, buildno, ofi_build_mode);
    if (os.path.exists(mpich_suite_build_path) == False):
        shutil.copytree(ci_site_config.mpich_src, mpich_suite_build_path)

    mpich_suite_path = '{}/test/'.format(mpich_suite_build_path)
    mpichsuite_installpath= "{}/mpichsuite/test".format(mpi_install_path)
    pwd = os.getcwd()
    if (mpi == 'impi'):
        os.chdir("{}/mpi".format(mpich_suite_path)) 
        cmd = ["./configure", "--with-mpi={}/intel64" \
               .format(ci_site_config.impi_root)]

        configure_cmd = shlex.split(" ".join(cmd))
        common.run_command(configure_cmd)
        common.run_command(["make", "all","-j32"])
        shutil.copytree(mpich_suite_path, mpichsuite_installpath)
        common.run_command(["make", "distclean"])
        os.chdir(pwd)



def build_stress_bm(mpi, mpi_install_path, libfab_install_path):
    
    stress_install_path = "{}/stress".format(mpi_install_path)
    if (os.path.exists(stress_install_path) == False):
        os.makedirs(stress_install_path)
     
    if (mpi == 'impi'):
        os.environ['LD_LIBRARY_PATH'] = "{}/lib".format(libfab_install_path)
        mpicc_path = "{}/intel64/bin/mpicc".format(ci_site_config.impi_root) 
    else:
        os.environ['LD_LIBRARY_PATH'] = ""
        mpicc_path = "{}/bin/mpicc".format(mpi_install_path)

    cmd=" ".join([mpicc_path, '-lz', "{}/mpi_stress/mpi_stress.c" \
                 .format(ci_site_config.benchmarks['wfr-mpi-tests']),\
                  '-o', "{}/mpi_stress".format(stress_install_path)])

    runcmd = shlex.split(cmd)
    common.run_command(runcmd)
 

def build_osu_bm(mpi, mpi_install_path, libfab_install_path):
    
    osu_install_path = "{}/osu".format(mpi_install_path)
    if (os.path.exists(osu_install_path) == False):
        os.makedirs(osu_install_path)
    os.chdir(osu_install_path)
    
    if (mpi == 'impi'):
        os.environ['CC']="{}/intel64/bin/mpicc".format(ci_site_config.impi_root)
        os.environ['CXX']="{}/intel64/bin/mpicxx".format(ci_site_config.impi_root)
        os.environ['LD_LIBRARY_PATH'] = "{}/lib".format(libfab_install_path)

    else: 
        os.environ['CC']="{}/bin/mpicc".format(mpi_install_path)
        os.environ['CXX']="{}/bin/mpicxx".format(mpi_install_path)
        os.environ['LD_LIBRARY_PATH']=""

    
    os.environ['CFLAGS']="-I{}/util/".format(ci_site_config.benchmarks['osu'])
    cmd = " ".join(["{}/configure".format(ci_site_config.benchmarks['osu']),
                    "--prefix={}".format(osu_install_path)])
   
    configure_cmd = shlex.split(cmd) 
    
    common.run_command(configure_cmd)
    common.run_command(["make", "-j4"])
    common.run_command(["make", "install"])


if __name__ == "__main__":
#read Jenkins environment variables
    # In Jenkins,  JOB_NAME  = 'ofi_libfabric/master' vs BRANCH_NAME = 'master' 
    # job name is better to use to distinguish between builds of different
    # jobs but with same branch name.
    jobname = os.environ['JOB_NAME']
    buildno = os.environ['BUILD_NUMBER']
    workspace = os.environ['WORKSPACE']



    parser = argparse.ArgumentParser()
    parser.add_argument("build_item", help="build libfabric or fabtests",
                         choices=['libfabric','fabtests', 'impi_benchmarks', \
                        'ompi_benchmarks', 'mpich_benchmarks', 'shmem'])
    parser.add_argument("--ofi_build_mode", help="select buildmode debug or dl", \
                        choices=['dbg','dl'])

    args = parser.parse_args()
    build_item = args.build_item

    if (args.ofi_build_mode):
        ofi_build_mode = args.ofi_build_mode
    else:
        ofi_build_mode = 'reg'



    install_path = "{installdir}/{jbname}/{bno}/{bmode}" \
                     .format(installdir=ci_site_config.install_dir,
                            jbname=jobname, bno=buildno,bmode=ofi_build_mode)

    p = re.compile('mpi*')

    if (build_item == 'libfabric'):
        build_libfabric(install_path, ofi_build_mode)

    elif (build_item == 'fabtests'):
        build_fabtests(install_path, ofi_build_mode)
   #if the build_item contains the string 'mpi'   
    elif (p.search(build_item)):
        mpi = build_item[:-11] #extract the mpitype from '*mpi*_benchmarks' build_item
        mpi_install_path = "{}/{}".format(install_path, mpi) 
    
        if (os.path.exists(mpi_install_path) == False):
            os.makedirs(mpi_install_path) 
        if (mpi != 'impi'):
            mpisrc = ci_site_config.mpich_src if mpi == 'mpich' \
                     else ci_site_config.ompi_src
            # only need to build ompi or mpich, impi is available as binary
            build_mpi(mpi, mpisrc, mpi_install_path, install_path, ofi_build_mode)
        
	# build mpich_test_suite
        build_mpich_suite(mpi, mpi_install_path, install_path, ofi_build_mode)
        # run stress and osu benchmarks for all mpitypes
        build_stress_bm(mpi, mpi_install_path, install_path)
        build_osu_bm(mpi, mpi_install_path, install_path)
    elif (build_item == 'shmem'):
        # build shmem
        shmem_dir = "{}/shmem".format(install_path)
        build_shmem(shmem_dir, install_path)
        build_ISx(shmem_dir)
        build_PRK(shmem_dir)
        build_uh(shmem_dir)
    


