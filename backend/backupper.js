// backupper.js
// Description: This file is used to backup the database..
//
var path = require('path');
const fs = require("fs"); // backup: filesystem, tarvitaan cronjobia varten
const { exec } = require("child_process"); // backup: backend viestii linukalle
const DB_USER = process.env.DB_USER
const DB_PASSWORD = process.env.DB_PASSWORD
const DB_NAME = process.env.DB_NAME


const BACKUP_DIR = "/backup"; // tallennetaan erilliseen kansioon koska github actions ajaa yli backend/backup kansion joten nyt pysyvät tallessa vaikka tulisi uusi versio backendistä
if (!fs.existsSync(BACKUP_DIR)) {
  fs.mkdirSync(BACKUP_DIR, { recursive: true });
}

function backupDatabase(callback) {  
  let datetime = new Date();
  let year = datetime.getFullYear();
  let month = ("0" + (datetime.getMonth() + 1)).slice(-2);
  let day = ("0" + datetime.getDate()).slice(-2);
  let backupFile = path.join(BACKUP_DIR, `backup_${year}_${month}_${day}.sql`);
  
  // salasanan sanitisaatio poisti salasanan erikoismerkit, sekä tietokannan sanitisaatio poisti tietokannan erikoismerkit (_)
  let backupCommand = 'mysqldump ' +'-u '+DB_USER +' -p' +DB_PASSWORD +' ' +DB_NAME +' > ' +backupFile;
  exec(backupCommand, (error, stdout, stderr) => {
    if (error) {
      console.error("Error executing command:", error.message);
      return;
    }
    if (stderr) {
      console.error("Command error:", stderr);
      return;
    }
    console.log("Backup database done");
    if (callback) callback(null, "Success");
  });
}
module.exports = {
  backupDatabase
};
