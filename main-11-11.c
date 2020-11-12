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
//int ports[] = {8080, 443, 80, 22};
//int ip[] = {8080, 443, 80, 22};

bdd inp_ports,inp_ip;

bdd create_ports(int arr[]){
    
    bdd y1,y2,y_port;
    int n =arr.size()/arr[0].size();
    
    y1 = uint16_to_bdd(arr[0], 0);
    if(n>1){
	    for(int i=1;i<n;i++)
	    	y2 = uint16_to_bdd(arr[i], 0);
	    	y_port = bdd_addref(bdd_apply(y1, y2, bddop_or));
	    }
    }else{
    	y_port = y1;
    }
    
    return y_port;
}

bdd create_ip(int arr[]){
    
    bdd y1,y2,y_ip;
    int n =arr.size()/arr[0].size();
    
    y1 = uint32_to_bdd(arr[0], 0);
    if(n>1){
	    for(int i=1;i<n;i++)
	    	y2 = uint32_to_bdd(arr[i], 0);
	    	y_ip = bdd_addref(bdd_apply(y1, y2, bddop_or));
	    }
    }else{
    	y_ip = y1;
    }
    
    return y_ip;
}


bool check_sub_ports(bdd r1_ports){
    
    bdd result;
    
    printf("\ninp_ports BDD: \n");
    bdd_printtable(inp_ports);

    result = bdd_addref(bdd_apply(r1_ports, inp_ports, bddop_and));

    printf("\nResult of r1_ports AND inp_ports BDD: \n");
    bdd_printtable(result);

    result = bdd_addref(bdd_apply(r1_ports, inp_ports, bddop_imp));

    printf("\nResult of r1_ports imp inp_ports BDD: \n");
    bdd_printtable(result);

    result = bdd_addref(bdd_apply(inp_ports, r1_ports, bddop_imp));

    printf("\nResult of inp_ports imp r1_ports BDD: \n");
    bdd_printtable(result);

    return result;

}

bool check_sub_ip(bdd r1_ip){
    
    bdd result;
    
    printf("\ninp_ip BDD: \n");
    bdd_printtable(inp_ip);

    result = bdd_addref(bdd_apply(r1_ip, inp_ip, bddop_and));

    printf("\nResult of r1_ip AND inp_ip BDD: \n");
    bdd_printtable(result);

    result = bdd_addref(bdd_apply(r1_ip, inp_ip, bddop_imp));

    printf("\nResult of r1_ip imp inp_ip BDD: \n");
    bdd_printtable(result);

    result = bdd_addref(bdd_apply(inp_ip, r1_ip, bddop_imp));

    printf("\nResult of inp_ip imp r1_ip BDD: \n");
    bdd_printtable(result);

    return result;
}

int main(){    
    
    bdd_init(500,100);
    bdd_setvarnum(50);
    int r1ports[] = {8080, 443, 80, 22};
    int inpports[] = { 443, 80};
    r1_ports = create_ports(r1ports);
    inp_ports = create_ports(inpports);
    
    result_r1 = check_sub_port(r1_ports);
 
    if(result == bdd_true()) {
        printf("passed rule1");
    }else{
        printf("failed rule1");
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

