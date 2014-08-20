#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>

#define dev_name "em2"
#define filename "onet.txt"
#define SIZE_ETHERNET 14
#define ETHERTYPE_IP    0x0800   
#define ETHERTYPE_ARP   0x0806   
#define ETHERTYPE_RARP   0x8035
#define ETHER_ADDR_LEN 6
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

FILE *fp;
long count;

  struct sniff_ethernet {
	   u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	   u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	   u_short ether_wtype; /* IP? ARP? RARP? etc */
    };

/* IP header */
    struct sniff_ip {
	   u_char ip_vhl;		/* version << 4 | header length >> 2 */
	   u_char ip_tos;		/* type of service */
	   u_short ip_len;		/* total length */
	   u_short ip_id;		/* identification */
	   u_short ip_off;		/* fragment offset field */
	   #define IP_RF 0x8000		/* reserved fragment flag */
	   #define IP_DF 0x4000		/* dont fragment flag */
	   #define IP_MF 0x2000		/* more fragments flag */
	   #define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	   u_char ip_ttl;		/* time to live */
	   u_char ip_p;		/* protocol */
	   u_short ip_sum;		/* checksum */
	   struct in_addr ip_src,ip_dst; /* source and dest address */
    };

  void init();
    void sigpipe_handle(int sign);
    /*该å°å?på?è????ip???ï?äp???ï?å?ï????å?*/
    void process_packet(u_char *args,const struct pcap_pkthdr *header, const u_char *packet); 
    /*该å°å?p???ä?udp???tcp???è?å?*/
    void process_ippacket(const struct pcap_pkthdr *header, const u_char *packet);
     void *loop_cap(void *args);

 void sigpipe_handle(int sign)
    {
    }
    
int main()
{
	 /*Setting the device*/
        char *dev, errbuf[PCAP_ERRBUF_SIZE];
	dev=dev_name;
	count=0;
        fp = fopen(filename,"w");

	signal(SIGPIPE,sigpipe_handle);
        
        /*Opening the device for sniffing*/
        pcap_t *handle;     
        handle=pcap_open_live(dev,BUFSIZ,1,1000000000,errbuf);
        if(handle==NULL){
            fprintf(stderr,"couldn't open device %s: %s\n", dev,errbuf);
            return(2);	        
        } 
	pcap_loop(handle,-1,process_packet,NULL);
}             

void process_packet(u_char *args,const struct pcap_pkthdr *header, const u_char *packet)
{
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip;
	u_int size_ip;
        ethernet = (struct sniff_ethernet*)(packet);
	u_short ether_ptype;

        ether_ptype=ethernet->ether_wtype; 

	 switch(ntohs(ether_ptype))
        {
            case ETHERTYPE_IP: 
		//printf("%ld\n", count++);
		ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
		fprintf(fp,"%u",(header->ts).tv_sec);
         	fprintf(fp,".%u\t",(header->ts).tv_usec);
         	fprintf(fp,"%s\t",inet_ntoa(ip->ip_src));
         	fprintf(fp,"%s\t",inet_ntoa(ip->ip_dst));	
         	fprintf(fp,"%u\n",ntohs(ip->ip_id));	
			
                break;
	    default:
		break;
        }          
}
