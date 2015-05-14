set serveroutput on
/*
This is the package body file. This include the full implementation of 
each procedure defined in the package header file. Includes exception 
handling and cursor definition code. 
*/
create or replace package body studentPackage as

/*
 SHOW - Procedures return all tuples from requested tables
*/
PROCEDURE show_classes(cur OUT sys_refcursor) IS
BEGIN
open cur for
select * from classes
order by classid;
END; 


PROCEDURE show_students(cur OUT sys_refcursor) IS
BEGIN
open cur for
select * from students
order by SID asc;
END; 

PROCEDURE show_courses(cur OUT sys_refcursor) IS
BEGIN
open cur for
select * from courses
order by dept_code, course# asc;
END;

PROCEDURE show_prereqs(cur OUT sys_refcursor) IS
BEGIN
open cur for
select * from prerequisites
order by dept_code, course# asc; 
END;

PROCEDURE show_enrollments(cur OUT sys_refcursor) IS
BEGIN
open cur for 
select * from enrollments
order by sid asc;
END;

PROCEDURE show_logs (cur OUT sys_refcursor) IS
BEGIN 
open cur for
select * from logs
order by logid desc; -- Most recent data is on top
END;

/*
 Procedures that perform exception checking and output messages 
*/
PROCEDURE add_students (st_id IN students.sid%TYPE,
fname IN students.firstname%TYPE,
lname IN students.lastname%TYPE,
st_status IN students.status%TYPE,
st_gpa IN students.gpa%TYPE,
st_email IN students.email%TYPE, hand_shake OUT INTEGER) IS

SQL_EXP_STU_ALREADY_PRESENT exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_STU_ALREADY_PRESENT, -20003);

sidcount INTEGER;
BEGIN
hand_shake := 0;

select count(*) into sidcount 
from students where sid = st_id;
if sidcount > 0 then
raise_application_error(-20003, 'The Student is already present in database');
end if;

insert into students values (st_id, fname, lname, st_status, st_gpa, st_email);
hand_shake := 1;

END;


procedure show_prereqinfo(dc IN courses.dept_code%TYPE, dcn IN courses.course#%TYPE, stud_cur
OUT sys_refcursor) IS
SQL_EXP_COURSE_DOES_NOT_EXISTS exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_COURSE_DOES_NOT_EXISTS, -20004);
err_no INTEGER;
BEGIN

select count(*) into err_no from courses where dept_code = dc and course# = dcn; 
if err_no < 1 then
raise_application_error(-20004, 'The course does not exist in course table');
end if;

open stud_cur for
select *
from prerequisites
start with
course# = dcn and dept_code = dc
connect by prior
        pre_course# = course#
        and prior pre_dept_code = dept_code;
END;




PROCEDURE show_studentinfo (st_id IN students.sid%TYPE, cur OUT sys_refcursor) 
IS
SQL_EXP_STUDENT_NO_COURSES exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NO_COURSES, -20002);
SQL_EXP_STUDENT_NOT_PRESENT exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NOT_PRESENT, -20001);
v_sid INTEGER;
v_sid2 INTEGER;
BEGIN

select count(*) into v_sid from students where sid = st_id;

if(v_sid < 1) then
raise_application_error(-20001, 'The SID is invalid');
end if;

select count(*) into v_sid2 from enrollments e, students s where e.sid = 
s.sid and s.sid =st_id;

if v_sid2 < 1 then
raise_application_error(-20002, 'The student has not taken any course.');
end if;


open cur for
select distinct s.sid, s.firstname, (c.dept_code||c.course#) as CourseName, c.title
from courses c, students s, classes cl, enrollments e
where c.dept_code = cl.dept_code and c.course# = cl.course# and e.sid = s.sid and e.classid
= cl.classid and s.sid = st_id;

END;


procedure show_classinfo(cid IN classes.classid%TYPE, cur OUT 
sys_refcursor)
IS
SQL_EXP_CLASSID_NOT_PRESENT exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_CLASSID_NOT_PRESENT, -20005);
cid_chk INTEGER;
BEGIN

select count(*) into cid_chk from classes where classid = cid;
if cid_chk < 1 then
raise_application_error(-20005, 'The CID is invalid.');
end if;

open cur for 
select cl.classid, co.title, s.sid, s.firstname
from courses co, classes cl, enrollments e, students s
where co.dept_code = cl.dept_code and co.course# = cl.course# and
s.sid = e.sid and e.classid = cl.classid and cl.classid =
cid;

END;

procedure enroll_student(sid_in IN students.sid%TYPE, cid IN 
classes.classid%TYPE, hand_shake OUT INTEGER, succ_msg OUT varchar2) IS

table_does_not_exist exception;  
PRAGMA EXCEPTION_INIT(table_does_not_exist, -942);

SQL_EXP_STU_COURSE_LIMIT_4 exception;
SQL_EXP_STUDENT_NOT_PRESENT exception;
SQL_EXP_CLASSID_NOT_PRESENT exception;
SQL_EXP_ALREADY_PRESENT exception;
SQL_EXP_STU_PREREQ_NOT_COMP exception;

PRAGMA EXCEPTION_INIT(SQL_EXP_STU_PREREQ_NOT_COMP, -20009);
PRAGMA EXCEPTION_INIT(SQL_EXP_STU_COURSE_LIMIT_4, -20008);
PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NOT_PRESENT, -20001);
PRAGMA EXCEPTION_INIT(SQL_EXP_CLASSID_NOT_PRESENT, -20005);
PRAGMA EXCEPTION_INIT(SQL_EXP_ALREADY_PRESENT, -20003);
err_chk INTEGER;
course_count INTEGER;

BEGIN
hand_shake := 0;
succ_msg := null;

select count(*) into err_chk from students where sid = sid_in;
if err_chk < 1 then 
raise_application_error(-20001, 'The sid is invalid');
end if; 

select count(*) into err_chk from classes where classid = cid;
if err_chk < 1 then
raise_application_error(-20005 ,'The classid is invalid.');
end if;

select count(*) into err_chk from enrollments e where e.sid = sid_in and 
e.classid = cid; 
if err_chk > 0 then
raise_application_error(-20003, 'The student is already in the class.');
end if;

select count(*) into course_count
from (( select cl.classid from classes cl, classes cl2
where cl.semester = cl2.semester and cl.year = cl2.year and cl2.year = (select
year from classes
where classid=cid and cl2.semester=(select semester from classes where
classid=cid)))
intersect
(select classid from enrollments where sid=sid_in));


if course_count = 3 then
	succ_msg := 'You are overloaded.';
elsif course_count = 4 then
raise_application_error(-20008, 'Students cannot be enrolled in more than four classes in the same semester.');
end if;

select count(*) into err_chk from ((select p.pre_dept_code, p.pre_course# from prerequisites p, 
classes cl
where cl.dept_code = p.dept_code and cl.course# = p.course# and cl.classid = cid)
minus 
(select c.dept_code, c.course# from classes c, enrollments e where e.sid 
= sid_in and e.classid = c.classid and e.lgrade is not null));

if err_chk > 0 then
	raise_application_error(-20009, 'Prerequisite courses have not been completed.');
end if;


insert into enrollments values (sid_in, cid, null);

hand_shake := 1;

END;

procedure drop_student_class(sid_in IN students.sid%TYPE, cid IN 
classes.classid%TYPE, hand_shake OUT INTEGER, succ_msg OUT varchar2) 
IS

SQL_EXP_STUDENT_NOT_PRESENT exception;
SQL_EXP_CLASSID_NOT_PRESENT exception;
SQL_EXP_STUDENT_NO_COURSES exception;

PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NOT_PRESENT, -20001);
PRAGMA EXCEPTION_INIT(SQL_EXP_CLASSID_NOT_PRESENT, -20005);
PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NO_COURSES, -20002);

err_chk INTEGER;

BEGIN
hand_shake := 0;
succ_msg := null;

select count(*) into err_chk from students where sid = sid_in;
if err_chk < 1 then
	raise_application_error(-20001, 'The sid is invalid');
end if;

select count(*) into err_chk from classes where classid = cid;
if err_chk < 1 then
	raise_application_error(-20005 ,'The classid is invalid.');
end if;

select count(*) into err_chk from enrollments e 
where e.sid = sid_in and e.classid = cid; 
if err_chk = 0 then
	raise_application_error(-20002, 'The student is not enrolled in the class');
end if;


delete from enrollments where classid = cid and sid = sid_in;

select count(*) into err_chk from enrollments where sid=sid_in;
if err_chk = 0 then
	succ_msg := 'This student is not enrolled in any classes.';
end if;

select count(*) into err_chk from enrollments where classid = cid;
if err_chk = 0 then
	if succ_msg = null then
		succ_msg := 'The class now has no students.';
	else
		succ_msg := succ_msg || ':' || 'The class now has no students.';
	end if;
end if;

hand_shake := 1;

END;

procedure delete_student(sid_in IN students.sid%TYPE, hand_shake OUT INTEGER)
IS
SQL_EXP_STUDENT_NOT_PRESENT exception;
PRAGMA EXCEPTION_INIT(SQL_EXP_STUDENT_NOT_PRESENT, -20001);
err_chk INTEGER;
BEGIN
hand_shake := 0;

select count(*) into err_chk from students where sid = sid_in;
if err_chk < 1 then
	raise_application_error(-20001, 'The sid is invalid');
end if;


delete from students where sid = sid_in;
hand_shake := 1;
END;

end studentPackage;
/
show errors

