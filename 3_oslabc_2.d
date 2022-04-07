proc:::exec-success
/uid == $1 /
{
 trace("\nAdmin");
 trace("\nProcess id(pid) is : ");
 trace(pid);
 trace("\nUser id(uid) is : ");
 trace(uid);
 trace("\nFiles used are : ");
 trace(curpsinfo->pr_psargs);
 trace("\n");
}
proc:::exec-success
/uid == $2 /
{
  printf("\nhead\n");
 trace("\nProcess id(pid) is : ");
 trace(pid);
 trace("\nUser id(uid) is : ");
 trace(uid);
 trace("\nFiles used are : ");
 trace(curpsinfo->pr_psargs);
 trace("\n");
}
proc:::exec-success
/uid == $3 /
{
  printf("\neducator\n");
 trace("\nProcess id(pid) is : ");
 trace(pid);
 trace("\nUser id(uid) is : ");
 trace(uid);
 trace("\nFiles used are : ");
 trace(curpsinfo->pr_psargs);
 trace("\n");
}
proc:::exec-success
/uid == $4 /
{
 printf("\nstudent\n");
 trace("\nProcess id(pid) is : ");
 trace(pid);
 trace("\nUser id(uid) is : ");
 trace(uid);
 trace("\nFiles used are : ");
 trace(curpsinfo->pr_psargs);
 trace("\n");
}
