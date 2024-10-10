#! /usr/bin/env python3

import os

templates = {}
templates['slurm-srun'] = '--job-name={}'
templates['slurm-gasnetrun_ibv'] = '-J {}'
templates['pbs-aprun'] = '-N {}'

def skipLauncherTest(launcher):
    return launcher not in templates

def getBatchFile(outputFile):
    with open(outputFile, 'r') as f:
        output = f.read()
        batchFile = output.split()[-1].strip("'")
    return (batchFile, output)


def checkLauncherArg(prog, outputFile, outputContent=None, delete=False):

    with open(prog + '.good', 'w') as fGood:
        prefix = os.getenv('CHPL_LAUNCHER_JOB_PREFIX', 'CHPL-')
        name = os.getenv('CHPL_LAUNCHER_JOB_NAME', prefix + prog[:10])
        launcher = os.getenv('CHPL_LAUNCHER', None)
        arg = None
        if launcher in templates:
            template = templates[launcher]
            if template != None:
                arg = template.format(name)
        else:
            fGood.write('*** Please update %s.prediff for launcher \'%s\'\n'
                        %(prog, os.getenv('CHPL_LAUNCHER')))

        with open(outputFile, 'r') as fExecOut:
            content = fExecOut.read()
            if arg is None or arg in content:
                if outputContent is not None:
                    fGood.write(outputContent)
                else:
                    fGood.write(content)
            else:
                fGood.write('Launcher argument missing: ' +  arg)
        if delete:
            # Delete the output file. Its name isn't fixed and .cleanfiles doesn't do globbing.
            os.unlink(outputFile)
