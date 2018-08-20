#!/usr/bin/python

import locale, sys

from optparse import OptionParser

class comm:
	def __init__(self, arguments, optu, opt1, opt2, opt3):
	        if arguments[0] == '-':
                        f1 = sys.stdin
                        f2 = open(arguments[1], 'r')
                elif arguments[1] == '-':
                        f1 = open(arguments[0], 'r')
			f2 = sys.stdin
		else:
			f1 = open(arguments[0], 'r')
			f2 = open(arguments[1], 'r')
        
		self.lines1 = f1.readlines()
                f1.close()
		self.lines2 = f2.readlines()
                f2.close()

                self.optunsorted = optu
		self.opt1 = opt1
		self.opt2 = opt2
		self.opt3 = opt3
            
                self.all = ""
    
        def finish(self):
                if len(self.lines2) > 0 and not self.opt1:
                        self.all += '\n'
                while len(self.lines1) > 0:
                        if not self.opt1:
                                self.all += (self.lines1[0])
                        del self.lines1[0]
                while len(self.lines2) > 0:
                        if not self.opt2:
                                self.all += ('\t' + self.lines2[0])
                        del self.lines2[0]

        def addline(self):
                abort = False
                while len(self.lines1) > 0 or len(self.lines2) > 0:
                        while self.lines1[0] < self.lines2[0]:
                                if not self.opt1:
                                        self.all+=(self.lines1[0])
                                del self.lines1[0]
                                if len(self.lines1) == 0:
                                        self.finish()
                                        abort = True
                                        break
                        if abort:
                                break
                        while self.lines1[0] == self.lines2[0]:
                                if not self.opt3:
                                        self.all+=('\t' + '\t' + self.lines1[0])
                                del self.lines1[0]
                                del self.lines2[0]
                                if len(self.lines1) == 0 or len(self.lines2) == 0:
                                        self.finish()
                                        abort = True
                                        break
                        if abort:
                                break
                        while self.lines1[0] > self.lines2[0]:
                                if not self.opt2:
                                        self.all+=('\t' + self.lines2[0])
                                del self.lines2[0]
                                if len(self.lines2) == 0:
                                        self.finish()
                                        abort = True
                                        break
                        if abort:
                                break
                        while self.lines1[0] == self.lines2[0]:
                                if not self.opt3:
                                        self.all+=('\t' + '\t' + self.lines1[0])
                                del self.lines1[0]
                                del self.lines2[0]
                                if len(self.lines1) == 0 or len(self.lines2) == 0:
                                        self.finish()
                                        abort = True
                                        break
                        if abort:
                                break

        def uaddline(self):
                while len(self.lines1) > 0:
                        count = 0
                        abort = False
                        while len(self.lines2) - count > 1:
                                if self.lines1[0] == self.lines2[count]:
                                        if not self.opt3:
                                                self.all += ('\t'+'\t'+self.lines1[0])
                                        del self.lines1[0]
                                        del self.lines2[count]
                                        abort = True
                                        break
                                count += 1
                        if not abort:
                                if not self.opt1:
                                        self.all += (self.lines1[0])
                                del self.lines1[0]
                self.finish()
        
        def printout(self):
                if self.optunsorted:
                        self.uaddline()
                else:
                        self.addline()
                sys.stdout.write(self.all)

def main():
        version_msg = "%prog 2.0"
	usage_msg = """%prog [OPTION]... FILE1 FILE2

Seperate lines from both files into three columns through comparison."""

        parser = OptionParser(version=version_msg,
                          usage=usage_msg)
        parser.add_option("-u", "--unsorted",
                      action="store_true", dest="unsorted", default=False,
                      help="For unsorted files (default False)")
        parser.add_option("-1",
                      action="store_true", dest="firstcol", default=False,
                      help="Do not output results of column 1 (default False)")
        parser.add_option("-2",
                      action="store_true", dest="secondcol", default=False,
                      help="Do not output results of column 2 (default False)")
        parser.add_option("-3",
                      action="store_true", dest="thirdcol", default=False,
                      help="Do not output results of column 3 (default False)")
	
        options, arguments = parser.parse_args(sys.argv[1:])
        
        try:
                optu = bool(options.unsorted)
        except:
                parser.error("Invalid UNSORTED: {0}".
                             format(options.unsorted))
        try:
		opt1 = bool(options.firstcol)
	except:
		parser.error("Invalid FIRST_COLUMN: {0}".
	   		     format(options.firstcol))
	try:
		opt2 = bool(options.secondcol)
	except:
		parser.error("Invalid SECOND_COLUMN: {0}".
	      		     format(options.secondcol))
	try:
		opt3 = bool(options.thirdcol)
	except:
		parser.error("Invalid THIRD COLUMN: {0}".
	       		     format(options.thirdcol))
                
	if len(arguments) != 2:
		parser.error("Invalid number of operands")
		
        if arguments[0] == '-' and arguments[1] == '-':
                parser.error("Both files cannot be from standard input")

	try:
		generator = comm(arguments, optu, opt1, opt2, opt3)
                generator.printout()
        except IOError as (errno, strerror):
                parser.error("I/O error({0}): {1}".
                             format(errno, strerror))

if __name__ == "__main__":
        main()
