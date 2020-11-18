#include <bdd.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include<stdio.h>
#include<string.h>

#define ll long long

int stoi_(char ch){
	int temp = ch - '0';
	return temp;
}

ll int dot_to_decimal(char s[]){
	int n = strlen(s);
    char arr[n+1];
    strcpy(arr,s);
    arr[n] = '.';
    
    long long int temp = 0,result= 0;
    for(int i=0;i<n+1;i++){
    	if(arr[i] == '.'){
			result = result<<8;
			result += temp;
			temp = 0;
		}else{
			temp = temp*10 + stoi_(s[i]);
		}
	}	
return result;
}

/* uint32_to_bdd: Function to convert a uint32 number
   into BDD.
   value: the number that you want to convert
   bitmask: how many LSBs are don't care (IP style mask)
*/
bdd uint32_to_bdd(uint32_t value, int bitmask)
{
    int i; /* the famous loop varaible i */
    bdd x[32]; /* variables. x[0] will hold MSB x[31] will hold LSB */
    bdd temp;  /* Store temporary results */
    uint32_t mask = 0x80000000;
    int significant_bits = 32 - bitmask; /* bits we care about */

    temp = bdd_true();

    for(i=0;i<significant_bits;i++) {
        if((value & mask) == 0) {
            x[i] = bdd_nithvar(i);
        } else {
            x[i] = bdd_ithvar(i);
        }
        temp = bdd_addref(bdd_apply(temp,x[i],bddop_and));
        //printf("\nBDD after adding bit: %d\n", i);
        //bdd_printtable(temp);
        value <<= 1;
    }
    return temp;
}
       
/* uint16_to_bdd: Function to convert a uint32 number
   into BDD.
   value: the number that you want to convert
   bitmask: how many LSBs are don't care (IP style mask)
*/
bdd uint16_to_bdd(uint16_t value, int bitmask)
{
    int i; /* the famous loop varaible i */
    bdd x[16]; /* variables. x[0] will hold MSB x[15] will hold LSB */
    bdd temp;  /* Store temporary results */
    uint16_t mask = 0x8000;
    int significant_bits = 16 - bitmask; /* bits we care about */

    temp = bdd_true();

    for(i=0;i<significant_bits;i++) {
        if((value & mask) == 0) {
            x[i] = bdd_nithvar(i);
        } else {
            x[i] = bdd_ithvar(i);
        }
        temp = bdd_addref(bdd_apply(temp,x[i],bddop_and));
#ifdef DEBUG
        printf("\nBDD after adding bit: %d\n", i);
        bdd_printtable(temp);
#endif
        value <<= 1;
    }
    return temp;
}

//Global var's
bdd inp_ports,inp_ip;

bdd create_ports(int arr[],int n){

    bdd y2,y_port;
    y_port = uint16_to_bdd(arr[0], 0);
    if(n>1){
	    for(int i=1;i<n;i++){
	    	y2 = uint16_to_bdd(arr[i], 0);
	    	y_port = bdd_addref(bdd_apply(y_port, y2, bddop_or));
	    }
    }
    
    return y_port;
}

bdd create_ip(ll int arr[],int n,int bitmask){
    
    bdd y2,y_ip;
    y_ip = uint32_to_bdd(arr[0], bitmask);
    if(n>1){
	    for(int i=1;i<n;i++){
	    	y2 = uint32_to_bdd(arr[i], bitmask);
	    	y_ip = bdd_addref(bdd_apply(y_ip, y2, bddop_or));
	    }
    }
    
    return y_ip;
}


bdd check_sub_ports(bdd r1_ports){
    
    bdd result;
    
    //printf("\ninp_ports BDD: \n");
    //bdd_printtable(inp_ports);

    result = bdd_addref(bdd_apply(inp_ports, r1_ports, bddop_imp));

    //printf("\nResult of inp_ports imp r1_ports BDD: \n");
    //bdd_printtable(result);

    return result;

}

bdd check_sub_ip(bdd r1_ip){
    
    bdd result;
    
    //printf("\ninp_ip BDD: \n");
    //bdd_printtable(inp_ip);

    result = bdd_addref(bdd_apply(inp_ip, r1_ip, bddop_imp));

    //printf("\nResult of inp_ip imp r1_ip BDD: \n");
    //bdd_printtable(result);

    return result;
}

int main(){    
    
    bdd_init(100000,100);
    bdd_setvarnum(50);
    
    /*int r1ports[] = {8080, 443, 80,22};
    int inpports[] = {22,8080};
    bdd r1_ports,result_r1;
    
    r1_ports = create_ports(r1ports,sizeof(r1ports)/sizeof(r1ports[0]));
    inp_ports = create_ports(inpports,sizeof(inpports)/sizeof(inpports[0]));
        
    result_r1 = check_sub_ports(r1_ports);
 
    if(result_r1 == bdd_true()) {
        printf("passed rule1\n");
    }else{
        printf("failed rule1\n");
    }*/
    
    ll int r1ip[1],inpip[1];
    char s_r[] = "10.0.1.12",s_in[] = "10.0.1.2";
    
    r1ip[0] =  dot_to_decimal(s_r);
    inpip[0] = dot_to_decimal(s_in);
    
    printf("result is: %lld\n",r1ip[0]);
    printf("result is: %lld\n",inpip[0]);

    bdd r1_ip,result_r1;
    int bitmask = 8;
 
    r1_ip = create_ip(r1ip,sizeof(r1ip)/sizeof(r1ip[0]),bitmask);
    inp_ip = create_ip(inpip,sizeof(inpip)/sizeof(inpip[0]),bitmask);
        
    result_r1 = check_sub_ip(r1_ip);
 
    if(result_r1 == bdd_true()) {
        printf("passed rule1\n");
    }else{
        printf("failed rule1\n");
    } 
}


/*
bdd acceptable_port(){
    z1 = uint16_to_bdd(8080, 0);
    z2 = uint16_to_bdd(443, 0);
    r1_ports = bdd_addref(bdd_apply(z1, z2, bddop_or));
    z1 = uint16_to_bdd(80, 0);
    r1_ports = bdd_addref(bdd_apply(r1_ports, z1, bddop_or));
    z2 = uint16_to_bdd(22, 0);
    r1_ports = bdd_addref(bdd_apply(r1_ports, z2, bddop_or));
    return r1_ports;
}


bdd acceptable_ip(){
    x1 = uint32_to_bdd(8080, 0);
    x2 = uint32_to_bdd(443, 0);
    r1_ip = bdd_addref(bdd_apply(x1, x2, bddop_or));
    x1 = uint32_to_bdd(80, 0);
    r1_ip = bdd_addref(bdd_apply(r1_ip, x1, bddop_or));
    x2 = uint32_to_bdd(22, 0);
    r1_ip = bdd_addref(bdd_apply(r1_ip, x2, bddop_or));
    return r1_ip;
}
*/

