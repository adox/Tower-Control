// EtherShield webserver demo
#include "EtherShield.h"

#define PWMPIN 6

// please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static uint8_t mymac[6] = {
  0x54,0x55,0x58,0x10,0x00,0x25}; 
  
static uint8_t myip[4] = {
  192,168,1,25};

#define MYWWWPORT 80
#define BUFFER_SIZE 1550
static uint8_t buf[BUFFER_SIZE+1];

// The ethernet shield
EtherShield es=EtherShield();

uint16_t http200ok(void)
{
  return(es.ES_fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n")));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
  uint16_t plen;
  plen=http200ok();
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("<html><head><title>Arduino ENC28J60 Ethernet Shield V1.0</title>"));
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js'></script></head><body>"));
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("<center><h1>Welcome to Arduino ENC28J60 Ethernet Shield V1.0</h1>"));	
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("<script type='text/javascript'>"));
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("$(document).ready(function(){$('#btnOn').click(function(){$.get('/?pwm=100');});"));
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("$('#btnOff').click(function(){$.get('/?pwm=10');});});"));
    plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("</script><input type='button' value='Off' id='btnOff'/><input type='button' value='On' id='btnOn'/></body></html>"));

  return(plen);
}


void setup(){

  // Initialise SPI interface
  es.ES_enc28j60SpiInit();

  // initialize enc28j60
  es.ES_enc28j60Init(mymac);

  // init the ethernet/ip layer:
  es.ES_init_ip_arp_udp_tcp(mymac,myip, MYWWWPORT);
  
  pinMode(PWMPIN, OUTPUT);
}

void loop(){
  uint16_t plen, dat_p;

  while(1) {
    // read packet, handle ping and wait for a tcp packet:
    dat_p=es.ES_packetloop_icmp_tcp(buf,es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf));

    /* dat_p will be unequal to zero if there is a valid 
     * http get */
    if(dat_p==0){
      // no http request
      continue;
    }
    // tcp port 80 begin
    if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
      // head, post and other methods:
      dat_p=http200ok();
      dat_p=es.ES_fill_tcp_data_p(buf,dat_p,PSTR("<h1>200 OK</h1>"));
      goto SENDTCP;
    }
    // just one web page in the "root directory" of the web server
    if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
      dat_p=print_webpage(buf);
      goto SENDTCP;
    }
    // pwm=x param service
    else if(strncmp("/?pwm=",(char *)&(buf[dat_p+4]),6)==0) {
      analogWrite(PWMPIN, atoi((char *)&buf[dat_p+10]));
      dat_p=http200ok();
      dat_p=es.ES_fill_tcp_data_p(buf,dat_p,PSTR("<h1>200 OK</h1>"));
      goto SENDTCP;      
    }    
    else{
      dat_p=es.ES_fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
      goto SENDTCP;
    }
SENDTCP:
    es.ES_www_server_reply(buf,dat_p); // send web page data
    // tcp port 80 end

  }

}


