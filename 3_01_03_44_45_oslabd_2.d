proc:::exec-success
/uid == $1 /
{
 trace("\nProcess id(pid) is : ");
 trace(pid);
 trace("\nUser id(uid) is : ");
 trace(uid);
 trace("\nProcesses executed/files opened are : ");
 trace(curpsinfo->pr_psargs);
 trace("\n");
}