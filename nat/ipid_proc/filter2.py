import sys, re
def lines(file):
	for line in file: yield line
def main():
	dic_ip = {}
	f = open('ip_list2.txt')
	lst_ip = f.readlines()
	f.close()
	for ip in lst_ip:
		dic_ip[ip.strip()] = 1
	for line in lines(sys.stdin):
		segs = re.split('\t', line.strip())
		if(dic_ip.has_key(segs[1].strip())):
			print line.strip()
				
			

main()
