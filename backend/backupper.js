// backupper.js
// Description: This file is used to backup the database.
//
var path = require('path');
const fs = require("fs"); // backup: filesystem, tarvitaan cronjobia varten
const { exec } = require("child_process"); // backup: backend viestii linukalle

const BACKUP_DIR = path.join(__dirname, "backup");
if (!fs.existsSync(BACKUP_DIR)) {
  fs.mkdirSync(BACKUP_DIR, { recursive: true });
}
const DB_USER = process.env.DB_USER;
const DB_PASSWORD = process.env.DB_PASSWORD;
const DB_NAME = process.env.DB_NAME;

function backupDatabase(callback) {  
  let datetime = new Date();
  let year = datetime.getFullYear();
  let month = ("0" + (datetime.getMonth() + 1)).slice(-2);
  let day = ("0" + datetime.getDate()).slice(-2);
  //let second = datetime.getSeconds(); //poista kun toimii ja backupfilerakenteesta
  //let filecount = 0;
  //console.log(backupCommand);
  //console.log(datetime);
  //console.log(year);
  //console.log(month);
  //console.log(day);
  //console.log(second);
  
  //normi backup ja joka sekunnin testeri (muista lisätä second muuttuja yllä):
  let backupFile = path.join(BACKUP_DIR, "backup_" +year+"_"+month+"_"+day+".sql");
  //let backupFile = path.join(BACKUP_DIR, "backup_" +year+"_"+month+"_"+day+second+".sql");
  
  let backupCommand = 'mysqldump -u '+DB_USER +' -p'+DB_PASSWORD+' '+DB_NAME +' > ' +backupFile;
  exec("ls backup/ -1 | wc -l", (error, stdout, stderr) => {
  if (error) {
    console.error("Error executing command:", error.message);
    return;
  }
  if (stderr) {
    console.error("Command error:", stderr);
    return;
  }
  let filecount = parseInt(stdout);
  //console.log("ls filecount: "+filecount);
  
  // 30 tiedostokopiota. Korvaa halutessa kahden alemman rivin nro.
  if (filecount > 30) {
    exec("ls backup/ -1t | tail -n +30 | xargs -i rm backup/{}", (error, stdout, stderr) => {
    if (error) {
      console.error("Error executing command:", error.message);
      return;
    }
    if (stderr) {
      console.error("Command error:", stderr);
      return;
    }
  });
}
  //console.log("filecount = " +filecount);
  exec(backupCommand, (error, stdout, stderr) => {
    if (error) {
      console.error(error.message);
      return;
    }
    else {
      console.log("Backup database done");
    }
  });
    //if (stderr) {
    //  console.error(stderr);
    //  return;
    //}
  
  });
}
console.log("Cron job started");


module.exports = {
  backupDatabase
};