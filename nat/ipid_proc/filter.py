import sys, re
def lines(file):
	for line in file: yield line
def main():
	for line in lines(sys.stdin):
		segs = re.split('\t', line.strip())
		if(re.search('192\.168\..+',segs[1]) and not re.search('192\.168\..+',segs[2])):
			# and not re.search('(192\.168\.10.174)|(192\.168\.1\.1)|(192\.168\.11\..+)',segs[1])):
			print line.strip()
				
			

main()
