//FK - Priority tasks
select Task, Project from tasks where Project = 'Boat';

//FK - Regroupement des assets
select AType, sum(Value) from assets where ALFlag = 'Asset' group by AType;

//FK - Regroupement par semaine
select year(MyDate), week(MyDate), sum(Credit) from cashflows where Category = 'Salary' group by year(MyDate), week(MyDate) order by year(MyDate), week(MyDate);


select * from assets where AType = 'Acquisition' order by Value desc;
select * from assets where AType = 'Acquisition';
select Category, sum(Debit), sum(Credit), DATE_FORMAT(MyDate,'%b') Month, DATE_FORMAT(MyDate,'%y') Year from cashflows group by MONTH(MyDate), Category;

//FK - Periodic cashflows
SELECT * FROM cashflows WHERE Monthly = 1 limit 10;

select DATE_FORMAT(MyDate,'%y') Year, DATE_FORMAT(MyDate,'%b') Month,Category, sum(Debit), sum(Credit) from cashflows group by MONTH(MyDate), YEAR(MyDate), Category order by Year, MONTH(MyDate), sum(Debit) desc;

//FK - On regroupe pour le GnuPlot des charges
select DATE_FORMAT(MyDate,'%y') Year, DATE_FORMAT(MyDate,'%b') Month,Category, sum(Debit), sum(Credit) from cashflows where MyDate > '2018-12-01' and MyDate < '2018-12-31' group by MONTH(MyDate), YEAR(MyDate), Category order by Year, MONTH(MyDate), sum(Debit) desc;

select DATE_FORMAT(MyDate,'%y') Year, DATE_FORMAT(MyDate,'%b') Month,Category, sum(Debit), sum(Credit) from cashflows where Category = 'Cash' group by MONTH(MyDate), YEAR(MyDate), Category order by Year, MONTH(MyDate), sum(Debit) desc;
//FK - PnL par mois
select month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate);
select month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate), sum(Debit);
//FK - Cashflows entre 2 dates
select * from cashflows where MyDate between '2015-01-04' and '2015-01-31' orderby Debit;
//FK - Highest debit per date
select * from cashflows where Debit > 500 order by MyDate desc;
//FK - Highest debit per debit
select * from cashflows where Debit > 500 order by Debit desc;
//FK - Movements for one category grouped by months
select month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows where Category = 'Cash' group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate);
//FK - Expenses by category between two dates
select * from cashflows where MyDate between '2015-01-04' and '2015-01-31' and Category = 'Cash' order by Debit desc;
//FK - Vérification du drift au cours d'un mois
select * from balance where MaxDate between '2015-01-01' and '2015-01-31' order by MaxDate;
//FK - Vérification de tout ce qui reste en l'air sans categorie
select * from cashflows where Category = 'Other';
//FK - Same mais avec seulement les positifs
select * from cashflows where Category = 'Other' and Credit > 0;

//FK - Vérification des taches
select Task, Project, Weight, Complexity, Duration, Status, Deliverable, Resource from tasks where Project = 'Boat' and status <> 'done' order by Weight desc;
//FK - A better one
select Task, Project, Weight, Complexity, Duration, Status, Deliverable, Resource from tasks where status <> 'done' order by Project, Weight desc;
//FK - Burndown
select day(Date), month(Date), year(Date), Project, sum(Weight) as RAF from burndown group by Project, date(Date), month(Date), year(Date) order by year(Date), month(Date), date(Date);
//FK - PnL par mois
select month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate);
//FK - Cashflows entre 2 dates
select * from cashflows where MyDate between '2015-01-04' and '2015-01-31' orderby Debit;
//FK - Highest debit per date
select * from cashflows where Debit > 500 order by MyDate desc;
//FK - Highest debit per debit
select * from cashflows where Debit > 500 order by Debit desc;
//FK - Movements for one category grouped by months
select month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows where Category = 'Cash' group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate);
//FK - Expenses by category between two dates
select * from cashflows where MyDate between '2015-01-04' and '2015-01-31' where Category = 'Cash' order by Debit desc;
//FK - Vérification du drift au cours d'un mois
select * from balance where MaxDate between '2015-01-01' and '2015-01-31' order by MaxDate;
//FK - Vérification de tout ce qui reste en l'air sans categorie
select * from cashflows where Category = 'Other';
//FK - Same mais avec seulement les positifs
select * from cashflows where Category = 'Other' and Credit > 0;

//FK - Vérification des taches
select Task, Project, Weight, Complexity, Duration, Status, Deliverable, Resource from tasks where Project = 'Boat' and status <> 'done' order by Weight desc;
//FK - Burndown
select day(Date), month(Date), year(Date), Project, sum(Weight) as RAF from burndown group by Project, date(Date), month(Date), year(Date) order by year(Date), month(Date), date(Date);


