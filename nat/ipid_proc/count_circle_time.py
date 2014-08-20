import sys, re
def lines(file):
	for line in file: yield line

class item:
    def __init__(self):
        self.min_time=sys.maxint
        self.max_time=0
        self.total=0

def main():
	dic_ip = {}
	f = open('ip_list2.txt')
	lst_ip = f.readlines()
	f.close()
	for ip in lst_ip:
		dic_ip[ip.strip()] = item()

	for line in lines(sys.stdin):
		segs = re.split('\t', line.strip())
		t = segs[0].strip()
		ip = segs[1].strip()
		if(dic_ip.has_key(ip)):
			dic_ip[ip].total += 1
			if(t < dic_ip[ip].min_time):
				dic_ip[ip].min_time = t
			if(t > dic_ip[ip].max_time):
				dic_ip[ip].max_time = t
				
	
	for key in dic_ip:
		print "%s\t%d\t%d\t" %{key, }	

main()
