import sys, re
def lines(file):
	for line in file: yield line
def main():
	dic_ip = {}
	f = open('ip_list.txt')
	lst_ip = f.readlines()
	f.close()
	for ip in lst_ip:
		dic_ip[ip.strip()] = []
	for line in lines(sys.stdin):
		segs = re.split('\t', line.strip())
		dic_ip[segs[1].strip()].append(int(segs[3].strip()));

	for ip in dic_ip:
		ipids = dic_ip[ip]
		exp  = 0
		for i in range(len(ipids) - 1):
			if ipids[i] - ipids[i+1] > 100 and  ipids[i] - ipids[i+1] < 20000 and ipids[i+1] > 1000:
				exp = exp + 1
		
		if exp < 1:
			print ip
main()
