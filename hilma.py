#!/usr/bin/python
# -*- coding: utf-8 -*-
# GENERAL MODULES
import os
import sys
import argparse
import multiprocessing
import datetime
import subprocess
import inspect


def main() :
    """Gathering arguments and running functions"""
    parser = argparse.ArgumentParser(description='HilMa is a simple app that outputs a graphical plot of coverage information.\ne.g. a samtools depth output file.')
    subparsers = parser.add_subparsers()
    sam = subparsers.add_parser('sam') # in case of reading a samtools depth information
    sam.add_argument('input',nargs=1,type=str,help='A valid file path to a samtools depth output.')
    sam.add_argument('name',nargs=1,type=str,help='ID (name) of the contig to plot.')
    sam.set_defaults(func=samread)

    custom = subparsers.add_parser('custom') # in case of reading a custom file depth information
    custom.add_argument('input',nargs=1,type=str,help='A valid table file path.')
    custom.add_argument('id_column',nargs=1,type=int,help='Column number (0 indexed) of the id.')
    custom.add_argument('value_column',nargs=1,type=int,help='Column number (0 indexed) of the coverage value per bin/basepair.')
    custom.add_argument('name',nargs=1,type=str,help='ID (name) of the contig/sequence/... to plot (found in table file in id_column).')
    custom.add_argument('--sep','-s',nargs=1,type=str,required=False,default=["\t"],help='Separation marker of table file. Default: tab character.')
    custom.set_defaults(func=customread)

    vector  = subparsers.add_parser('vector') # in case of reading a vector from stdin (from awk, R, etc)
    vector.add_argument('--name','-n',nargs=1,type=str,default=["vector"],required=False,help='Name of the current vector. Default: \'vector\'.')
    vector.set_defaults(func=stdinread)

    args = parser.parse_args()
    args.func(args)
    sys.exit()

def stdinread(args) :
    # CHANGE WORKING DIR TO HILMA DIRECTORY
    HILMApath = os.path.abspath(inspect.stack()[0][1])
    dirHILMAPath = os.path.dirname(os.path.abspath(inspect.stack()[0][1]))
    os.chdir(dirHILMAPath)
    # RUN SUBPROCESS
    command = "./HilMa - {}".format(args.name[0])
    proc = subprocess.Popen(command, shell=True, stdin=sys.stdin, stdout=subprocess.PIPE)
    while True :                                            # Waits and prints cout
        line = proc.stdout.readline()                       # Reads line from stdout
        if line.strip() == "" :                             # If line is empty
            pass
        else :                                              # Else prints the line
            if len(line) > 0 :
                print(line.decode("utf-8").strip())
        if not line :
            break                                           # If there is no piping in anymore
        proc.wait()                                         # Waits for a new piping in

def samread(args) :
    input = args.input[0]
    name = args.name[0]
    customread(None, input, name)

def customread(args, input=None, name=None) :
    if args != None : # FROM custom
        input = args.input[0]
        idcol = args.id_column[0]
        valcol = args.value_column[0]
        name = args.name[0]
        sepmark = args.sep[0]
    else : # FROM sam
        idcol = 0
        valcol = 2
        sepmark = "\t"
        # name from arg of function
        # input from arg of function

    if checkFile(input) :
        path = os.path.abspath(input)
    else :
        raise Exception("ERROR: {} is not a valid file path!".format(input))
    # CHANGE WORKING DIR TO HILMA DIRECTORY
    HILMApath = os.path.abspath(inspect.stack()[0][1])
    dirHILMAPath = os.path.dirname(os.path.abspath(inspect.stack()[0][1]))
    os.chdir(dirHILMAPath)

    # RUN SUBPROCESS
    command = "./HilMa - {}".format(name)
    proc = subprocess.Popen(command, shell=True, stdin=subprocess.PIPE, stdout=sys.stdout)
    f = open(input, "r")
    for line in f :
        s = line.split(sepmark)
        while s[idcol] != name :
            continue
        else :
            proc.stdin.write(s[valcol].encode('utf-8'))          # SEND value (and name) to stdin

    out, err = proc.communicate()
    proc.stdin.close()


def checkFile(path) :
    if path == None :
        raise Exception("ERROR: File path appears empty!")
    else :
        try :
            if os.path.isfile(path) : # Try avoids errors thrown if string is not pathlike
                return True
            else :
                raise Exception("ERROR: Invalid path file path!")
        except :
            raise Exception("ERROR: String does not look like a file path!") # raises exception error
            return False

if __name__ == '__main__':
    main()
