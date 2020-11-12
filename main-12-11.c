#include <bdd.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

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
        printf("\nBDD after adding bit: %d\n", i);
        bdd_printtable(temp);
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

bdd create_ip(int arr[],int n){
    
    bdd y2,y_ip;
    
    y_ip = uint32_to_bdd(arr[0], 0);
    if(n>1){
	    for(int i=1;i<n;i++){
	    	y2 = uint32_to_bdd(arr[i], 0);
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
    
    bdd_init(1000,100);
    bdd_setvarnum(50);
    
    int r1ports[] = {8080, 443, 80,22};
    int inpports[] = {22,8080};
    bdd r1_ports,result_r1;
    
    r1_ports = create_ports(r1ports,sizeof(r1ports)/sizeof(r1ports[0]));
    inp_ports = create_ports(inpports,sizeof(inpports)/sizeof(inpports[0]));
        
    result_r1 = check_sub_ports(r1_ports);
 
    if(result_r1 == bdd_true()) {
        printf("passed rule1\n");
    }else{
        printf("failed rule1\n");
    }
}

