// backupper.js
// Description: This file is used to backup the database.
//
var path = require('path');
const fs = require("fs"); // backup: filesystem, tarvitaan cronjobia varten
const { exec } = require("child_process"); // backup: backend viestii linukalle
const DB_USER = process.env.DB_USER
const DB_PASSWORD = process.env.DB_PASSWORD
const DB_NAME = process.env.DB_NAME


const BACKUP_DIR = path.join(__dirname, "backup");
if (!fs.existsSync(BACKUP_DIR)) {
  fs.mkdirSync(BACKUP_DIR, { recursive: true });
}

function backupDatabase(callback) {  
  let datetime = new Date();
  let year = datetime.getFullYear();
  let month = ("0" + (datetime.getMonth() + 1)).slice(-2);
  let day = ("0" + datetime.getDate()).slice(-2);
  let backupFile = path.join(BACKUP_DIR, `backup_${year}_${month}_${day}.sql`);
  
  const sanitizedUser = DB_USER.replace(/[^a-zA-Z0-9_]/g, '');
  const sanitizedPassword = DB_PASSWORD.replace(/[^a-zA-Z0-9_]/g, '');
  const sanitizedDbName = DB_NAME.replace(/[^a-zA-Z0-9_]/g, '');
  const sanitizedBackupFile = backupFile.replace(/[^a-zA-Z0-9_/.]/g, '');
  
  let backupCommand = 'mysqldump ' +'-u '+sanitizedUser +' -p' +sanitizedPassword +' ' +sanitizedDbName +' > ' +sanitizedBackupFile;
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
