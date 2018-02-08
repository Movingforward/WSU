.global main, mymain, myprintf

main:
    #Stack Instantiation
    pushl %ebp 
    movl %esp, %ebp
    
    pushl 16(%ebp) #push env
    pushl 12(%ebp) #push argv
    pushl 8(%ebp)  #push argc
    call mymain
    addl $16, %esp #Move ESP to mymain on stack

    movl %ebp, %esp 
    popl %ebp   #popping contents off stack
    ret
