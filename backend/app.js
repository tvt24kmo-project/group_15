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
//backup-osat alla. Cron täytyy asentaa: npm install node-cron
const { exec } = require("child_process"); // backup: backend viestii linukalle
const cron = require("node-cron"); // backup: kello
const fs = require("fs"); // backup: filesystem, tarvitaan cronjobia varten



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


//backup koodi alkaa:
const BACKUP_DIR = path.join(__dirname, "backup");
if (!fs.existsSync(BACKUP_DIR)) {
  fs.mkdirSync(BACKUP_DIR, { recursive: true });
}
const DB_USER = process.env.DB_USER;
const DB_PASSWORD = process.env.DB_PASSWORD;
const DB_NAME = process.env.DB_NAME;
const backupFile = path.join(BACKUP_DIR, "backup.sql");
const backupCommand = 'mysqldump -u '+DB_USER +' -p'+DB_PASSWORD+' '+DB_NAME +' > ' +backupFile;

function backupDatabase() {  
  console.log(backupCommand);
  exec(backupCommand, (error, stdout, stderr) => {
    if (error) {
      console.error(error.message);
      return;
    }
    //if (stderr) {
    //  console.error(stderr);
    //  return;
    //}
  console.log("Backup database done");
  });
}
cron.schedule("0 3 * * *", backupDatabase); // backupataan joka päivä klo 03:00. ("* * * * * *"), on joka sekunti
//backup koodi loppu



module.exports = app;
