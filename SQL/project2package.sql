set serveroutput off

/*
This is the package header declaration and includes all procedure header 
definitions. The full implementation of these procedures is found in the 
Package body. 

*/
create or replace package studentPackage as
type ref_cursor is ref cursor; 

procedure show_students(cur OUT sys_refcursor);
procedure show_courses(cur OUT sys_refcursor);
procedure show_prereqs(cur OUT sys_refcursor);
procedure show_classes(cur OUT sys_refcursor);
procedure show_enrollments(cur OUT sys_refcursor);
procedure show_logs(cur OUT sys_refcursor);


procedure add_students (st_id IN students.sid%TYPE,
fname IN students.firstname%TYPE,
lname IN students.lastname%TYPE,
st_status IN students.status%TYPE,
st_gpa IN students.gpa%TYPE,
st_email IN students.email%TYPE, hand_shake OUT INTEGER);

procedure show_studentinfo(st_id IN students.sid%TYPE, cur OUT sys_refcursor);  
procedure show_prereqinfo(dc IN courses.dept_code%TYPE, dcn IN courses.course#%TYPE, stud_cur 
OUT sys_refcursor);
procedure show_classinfo(cid IN classes.classid%TYPE, cur OUT 
sys_refcursor); 

procedure enroll_student(sid_in IN students.sid%TYPE, cid IN classes.classid%TYPE, hand_shake OUT INTEGER, succ_msg OUT varchar2);

procedure drop_student_class(sid_in IN students.sid%TYPE, cid IN 
classes.classid%TYPE, hand_shake OUT INTEGER, succ_msg OUT varchar2);

procedure delete_student(sid_in IN students.sid%TYPE, hand_shake OUT INTEGER);
 
end studentPackage;
/
show errors
