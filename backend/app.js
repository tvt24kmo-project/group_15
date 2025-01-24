var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/customers');
var cardsRouter = require('./routes/cards');
var transactionsRouter = require('./routes/transactions');
var accountsRouter = require('./routes/accounts');


var app = express(); 

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended:false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));


// MUISTA LISÄTÄ ROUTERIT JA NIIDEN POLUT TÄNNE ETTÄ EI TULE 404 VIRHETTÄ
app.use('/', indexRouter);
app.use('/customers', usersRouter);
app.use('/cards', cardsRouter);
app.use('/transactions', transactionsRouter);
app.use('/accounts', accountsRouter);

app.use((req, res, next) => {
    console.log(`Incoming request: ${req.method} ${req.path}`);
    next();
});


module.exports = app;
