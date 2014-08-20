#include <vector>
#include <cstdio>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <set>
#include <list>

using namespace std;


#define FNAME "ipid2.txt"
#define MAX_LINE 1024
#define MAX_ID 32767
#define GAP_LIM  64
#define GAP_FAC 16
#define TIM_LIM 120
#define TIM_FAC 1
#define F_SIZE 50
#define SLOT 600


typedef unsigned int uint;
typedef unsigned short ushort;

struct ipid{
	long tm;
	long val;
	ipid(uint tm, ushort val):tm(tm), val(val){}
};

void split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	substring = str.substr(start);
	dest.push_back(substring);
}



class Sqs{
	public:
		vector<ipid> ipids;
		vector<vector<ipid> > sqs;
		vector<int> sq_size;
		vector<list<int> > sq_num;

		/*get initial sqs*/
		void get_init_sqs()
		{

			int n = ipids.size();

			for(int i = 0; i < n; i++)
			{

				int pos = -1;
				int m = sqs.size();
				long min_tm_gap = TIM_LIM;
				long min_val_gap = GAP_LIM;

				if(ipids[i].val == 0) continue;
				for(int j = 0; j < m; j++)
				{
					int last = sqs[j].size() - 1;
					long t_tm = abs(sqs[j][last].tm - ipids[i].tm);
					long t_val = abs(sqs[j][last].val - ipids[i].val);
					long t_lp = abs(abs(sqs[j][last].val - ipids[i].val) - MAX_ID);
					if(t_tm < min_tm_gap && (t_val < min_val_gap || t_lp < min_val_gap))
					{
						pos = j;
						min_tm_gap = t_tm;
						min_val_gap = min(t_val, t_lp);
					}
				}

				if(pos != -1)
				{
					sqs[pos].push_back(ipids[i]);
				}
				else
				{
					vector<ipid> sq;
					sq.push_back(ipids[i]);
					sqs.push_back(sq);
				}
			}
		}
		
		/*merge sqss that are adjacent*/	
		void merge_sqs()
		{

			for(int i = 0; i < sqs.size(); i++)
			{
				sq_size.push_back(sqs[i].size());
				list<int> lst;
				lst.push_back(i);
				sq_num.push_back(lst);
			}

			while(1)
			{
				int flag = 0;
				for(int i = 0; i < sq_num.size(); i++)
				{
					for(int j = i+1; j < sq_num.size();j++)
					{
						if(i == j) continue;
						if(sq_num[i].size() == 0 || sq_num[j].size() == 0)
							continue;

						list<int>::reverse_iterator x = sq_num[i].rbegin();
						list<int>::iterator y = sq_num[j].begin();

						int ii = min(*x, *y);
						int jj = max(*x, *y);	
						int last = sqs[ii].size() - 1;
						long t_tm = sqs[jj][0].tm - sqs[ii][last].tm;
						long t_gap = sqs[jj][0].val - sqs[ii][last].val;

						if((t_tm > 0 && t_tm < TIM_FAC*TIM_LIM && t_gap > 0 && t_gap < GAP_FAC*GAP_LIM))
						{
							flag = 1;
							sq_num[i].insert(sq_num[i].end(), sq_num[j].begin(), sq_num[j].end());
							sq_num[j].clear();
							goto TESTING;
						}

					}
				}
				TESTING:
					if(flag == 0)
						break;
			}
		}
		
		/*discard sqss that are too short*/
		int get_last_sqs()
		{
			int count = 0;
			for(int y = 0; y < sq_num.size(); y++)
			{
				if(sq_num[y].size() > 0)
				{
					int total = 0;
					for (list<int>::iterator x = sq_num[y].begin(); x != sq_num[y].end(); ++x)
					{
						//cout<<*x;
						total += sqs[*x].size();						
					}
					//cout<<endl;
					if(total >= F_SIZE)
						count++;
				}
			}
			
			return count;

		}

		/*get ipids from file, get the ct_slotTH part of ipids*/
		int get_ipids(char fname[], int ct_slot) 		
		{

			FILE *f = fopen(fname, "r");
			char line[MAX_LINE] = {0};
			set<string> ip_set;
			long start  = -1;
			long end = -1;
			
			while(fgets(line, sizeof(line), f))
			{
				vector<string> segs;
				string src(line);
				split(src, "\t", segs);
				if(start == -1)
					start = strtoul(segs[0].c_str(), NULL, 10);
				end = strtoul(segs[0].c_str(), NULL, 10);

				if((end - start >=  SLOT*ct_slot) && (end - start < SLOT *(ct_slot + 1)))
				{
					ip_set.insert(segs[1]);
					ipid e(strtoul(segs[0].c_str(), NULL, 10), strtoul(segs[3].c_str(), NULL, 10));
					ipids.push_back(e);
				}

			}


			fclose(f);	
			return ip_set.size();	
		}
};

class Solution{
	public:
		Sqs s;
		void ct_hosts_num()
		{
			char fname[] = FNAME;
			int ct_slot = 0;
			for(;;ct_slot++)	
			{
				cout<<"time slot:"<<ct_slot<<endl;
				s.ipids.clear();
				s.sqs.clear();
				s.sq_size.clear();
				s.sq_num.clear();

				int real_num = s.get_ipids(fname, ct_slot);
				if(s.ipids.size() == 0)	
					break;

				cout<<"real num of hosts:"<<real_num<<endl;

				s.get_init_sqs();
				cout<<"sqs num before merge:"<<s.sqs.size()<<endl;			

				s.merge_sqs();
				int num = s.get_last_sqs();
				cout<<"sqs num after merge:"<<num<<endl;
			}
			
		}
			
};

int main()
{
	Solution s;
	s.ct_hosts_num();
	return 0;
}
