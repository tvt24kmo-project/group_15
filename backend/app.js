var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var bodyParser = require('body-parser');
var app = express(); 


var indexRouter = require('./routes/index');
var usersRouter = require('./routes/customers');
var cardsRouter = require('./routes/cards');
var transactionsRouter = require('./routes/transactions');
var accountsRouter = require('./routes/accounts');
var loginRouter = require('./routes/login');
const jwt = require('jsonwebtoken');
var proceduresRouter = require('./routes/procedures');
//backup-osat:
const backupper = require('./backupper');
const cron = require("node-cron"); // backup: ajoitus
// backup-osat loppu.






app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended:false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


function authenticateToken(req, res, next) {
  const unprotectedPaths = [ // luodaan lista reiteistä, jotka eivät vaadi kirjautumista (eli tokenia -- väärällä pinnillä kirjautuessa ei saa tokenia ja näin ollen ei pääse näihin reitteihin normaalisti, nyt ne ei ole suojattu enää)
    '/cards/check-card-attempts',
    '/cards/update-card-attempts',
    '/cards/check-lock-status'
  ];

  if (unprotectedPaths.some(path => req.path.startsWith(path))) {
    return next();
  }


    const authHeader = req.headers['authorization']
    const token = authHeader && authHeader.split(' ')[1]
  
    console.log("token = "+token);
    if (token == null) return res.sendStatus(401)
  
    jwt.verify(token, process.env.MY_TOKEN, function(err, user) {
  
      if (err) return res.sendStatus(403)

      req.user = user
  
      next()
    })
}


// MUISTA LISÄTÄ ROUTERIT JA NIIDEN POLUT TÄNNE ETTÄ EI TULE 404 VIRHETTÄ
app.use('/', indexRouter);
app.use('/login', loginRouter);
app.use('/cards', cardsRouter);
app.use(authenticateToken); // KOMMENTOI TÄMÄ RIVI POIS JOS HALUAT TESTATA ILMAN KIRJAUTUMISTA
app.use('/customers', usersRouter);
app.use('/transactions', transactionsRouter);
app.use('/accounts', accountsRouter);
app.use('/procedures', proceduresRouter);

app.use((req, res, next) => {
    console.log(`Incoming request: ${req.method} ${req.path}`);
    next();
});

//backup ajoitettu alkaa:
cron.schedule("0 3 * * *", backupper.backupDatabase); // backupataan joka päivä klo 03:00. ("* * * * * *"), on joka sekunti
//backup ajoitettu loppu.


module.exports = app;
