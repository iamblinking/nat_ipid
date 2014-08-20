import sys, re
def lines(file):
	for line in file: yield line
def main():
	for line in lines(sys.stdin):
		segs = re.split('\t', line.strip())
		#sub_segs = re.split('\.', segs[0].strip())
		#print sub_segs[0].strip(),"\t",segs[1].strip(),"\t", segs[2].strip(),"\t", segs[3].strip()
		print "%s\t%s\t%s\t%s" %(segs[0].strip(),segs[1].strip(),segs[2].strip(),segs[3].strip())
				
			

main()
