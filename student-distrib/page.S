.globl ENABLE_PAGING
ENABLE_PAGING:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax             
	mov %eax, %cr3      

	mov %cr4, %eax          
	orl $0x00000010, %eax     
	mov %eax, %cr4       
 
    leave
    ret


.globl flshtlb
flshtlb:
	ret



.global PSE_Setter
PSE_Setter:
	mov %cr0, %eax          
	orl $0x80000000, %eax     
	mov %eax, %cr0  
	ret

 
	      