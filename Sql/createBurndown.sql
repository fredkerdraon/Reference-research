create table burndown(
       	Date datetime, 
        Status varchar(25),
        Project varchar(25),
        Weight real,
	Velocity real,
	Target real 
);

ALTER IGNORE TABLE burndown 
ADD UNIQUE INDEX idx_burndown_project (Date, Project);

