BEGIN     /*prints hello world, initializations can be done*/
{
  trace("HEllo World\n");
  i = 0;
}

syscall::open:entry /* prints arg0 whenever dtrace command is not  executed */
/ execname != "dtrace"  /
{
  printf("\n1\n");
  printf("%s\n",copyinstr(arg0));
}

proc:::exec-success  /* whenever there is a successful process executed */

{
 printf("\n2\n");
 trace(curpsinfo->pr_psargs);
 printf("\n");
}

/*
syscall::read:return   to trace errors and show error number 
/ arg0 < 0 /
{
  printf("\n3\n");
  trace(errno);
  printf("\n");
}
*/

syscall::fork*:    /* fork() syscalls can be traced and pid can be known */
{
  printf("\n4\n");
  trace(pid);
  printf("\n");
}

syscall:::entry    /* @=aggregate, used to count syscalls from ls */ 
/execname == "ls" /
{
  @[probefunc] = count();
}

profile-997hz      /* profiling processes on CPU running user code */
/ arg1 /
{
  @[execname] = count()
}

sched:::on-cpu    /* processes getting on cpu */ 
{
  printf("\n5\n");
  trace(execname);
  printf("\n");
}

profile:::tick-1sec    /* profile provider will fire a probe every 1 sec */
/ i >= 0 /
{
  i = i+1;
  trace(i); 
}

syscall::read:entry,      /* goes to the action(prints probe functions) when pid = 7650 */
syscall::write:entry
/ pid == 7650 /
{
  printf("\n6\n");
  trace(probefunc);
  printf("\n");
}

syscall::chdir:entry    /* can trace the current working directory and cd directory */
{
   printf("\n7\n");
   printf(" %s",copyinstr(arg0));
   printf("\n");
} 
