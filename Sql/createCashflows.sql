create table cashflows(
        MyDate datetime,
        GroupLib varchar(50),
        Libelle varchar(50),
        Debit double,
        Credit double,
        Category varchar(25),
        Weekly bool,
        Monthly bool,
        Yearly bool
);

ALTER IGNORE TABLE cashflows
ADD UNIQUE INDEX idx_cashflows_debit (MyDate, Libelle, Debit);
ALTER IGNORE TABLE cashflows
ADD UNIQUE INDEX idx_cashflows_credit (MyDate, Libelle, Credit);

