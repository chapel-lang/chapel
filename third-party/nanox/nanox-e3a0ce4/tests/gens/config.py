#!/usr/bin/python
import os

def cross(*args):
	ans = [[]]
	for arg in args:
		ans = [x+[y] for x in ans for y in arg]
	return ans

def cpus(max_cpus):
	ans=[]
	for i in range(1,max_cpus+1):
		ans = ans + ['--pes='+str(i)]
	return ans

test_mode=os.environ.get('NX_TEST_MODE')
if test_mode == None:
	test_mode='small'

max_cpus=os.environ.get('NX_TEST_MAX_CPUS')
if ( max_cpus == None ):
	max_cpus=2

# Process program arguments (priority to env vars)
from optparse import OptionParser
import sys

header ='Nanox config generator 0.1\n\n'+\
	'Envorionment variables that affect this script:\n'+\
	'   NX_TEST_MODE=\'small\'|\'medium\'|\'large\'   -  \'small\' by default\n'+\
	'   NX_TEST_MAX_CPUS=#CPUS                  -  2 by default\n'
if '-h' in sys.argv or '--help' in sys.argv:
	print header

usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-a", metavar="\"a1|a2,b1|b2,..\"", dest="additional",
                  help="Comma separated lists of aditional options ('|' separates incompatible alternatives ) combined in the configurations generated")
parser.add_option("-m", choices=['small','medium','large'], dest="mode",
                  help="Determines the number of execution versions for each test combining different runtime options.")
parser.add_option("-c","--cpus", metavar="n", type='int', dest="cpus",
                  help="Each configuration will be tested for 1 to n CPUS")

(options, args) = parser.parse_args()

if len(args) != 0:
	parser.error("this script takes no arguments")

addlist=[]
if options.additional:
	additional=options.additional
	additional=additional.split(',')
	for a in additional:
		addlist=addlist+[a.split('|')]
if options.mode:
	test_mode=options.mode
if options.cpus:
	max_cpus=options.cpus



max_cpus=int(max_cpus)

scheduling_small=['--schedule=default','--schedule=affinity']
#scheduling_small=['--schedule=default','--schedule=dbf','--schedule=wf','--schedule=cilk']

scheduling_full=['--schedule=default','--schedule=bf --bf-stack','--schedule=bf --no-bf-stack','--schedule=dbf', '--schedule=affinity']
#scheduling_small=['--schedule=bf','--schedule=wf','--schedule=dbf','--schedule=cilk']
#scheduling_full=['--schedule=bf --bf-stack','--schedule=bf --no-bf-stack', '--schedule=wf --steal-parent --wf-local-policy=FIFO --wf-steal-policy=FIFO','--schedule=wf --steal-parent --wf-local-policy=FIFO --wf-steal-policy=LIFO','--schedule=wf --steal-parent --wf-local-policy=LIFO --wf-steal-policy=FIFO','--schedule=wf --steal-parent --wf-local-policy=LIFO --wf-steal-policy=LIFO','--schedule=wf --no-steal-parent --wf-local-policy=FIFO --wf-steal-policy=FIFO', '--schedule=wf --no-steal-parent --wf-local-policy=FIFO --wf-steal-policy=LIFO','--schedule=wf --no-steal-parent --wf-local-policy=LIFO --wf-steal-policy=FIFO','--schedule=wf --no-steal-parent --wf-local-policy=LIFO --wf-steal-policy=LIFO','--schedule=dbf','--schedule=cilk']
throttle=['--throttle=dummy','--throttle=idlethreads','--throttle=numtasks','--throttle=readytasks','--throttle=taskdepth']
#barriers=['--barrier=centralized','--barrier=tree','--barrier=dissemination']
barriers=['--barrier=centralized','--barrier=tree']
others=[cpus(max_cpus),['--disable-binding','--no-disable-binding'],['--architecture=smp','--architecture=smp-numa']]

if test_mode == 'small':
	configs=cross(*others+[scheduling_small]+addlist)
elif test_mode == 'medium':
	configs=cross(*others+[scheduling_small]+[throttle]+[barriers]+addlist)
elif test_mode == 'large':
	configs=cross(*others+[scheduling_full]+[throttle]+[barriers]+addlist)

config_lines=[]
versions=''
i=1
for c in configs:
	line = 'test_ENV_ver'+str(i)+'=\"NX_ARGS=\''
	versions+='ver'+str(i)+' '
	for entry in c:
		line = line + ' ' +entry
	line = line + '\'\"'
	config_lines += [line]
	i+=1

print 'exec_versions=\"'+ versions +'\"'
for line in config_lines:
	print line

