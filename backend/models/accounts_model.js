

const db = require('../database');

const accounts = {

    // lisää uusi tili tietokantaan
    addAccount: function (account, callback) {
        return db.query("INSERT INTO accounts (idaccount, cardnumber, debitbalance, creditbalance, idcard) VALUES(?, ?, ?, ?, ?)", [account.idaccount, account.cardnumber, account.debitbalance, account.creditbalance, idcard], callback);
    },
    // Listaa tilien tiedott
    getAllAccounts: function (callback) {
        return db.query("SELECT * FROM accounts", callback);
    },
    // Päivitä tili id:n perusteella
    updateAccount: function (id, callback) {
        return db.query("UPDATE accounts SET idaccount=?, cardnumber=?, debitbalance=?, creditbalance=?, idcard=? WHERE idaccount=?", [accounts.idaccount, accounts.cardnumber, accounts.debitbalance, accounts.creditbalance, accounts.idcard, id], callback);
    },
    // Poista tili id:n perusteella
    deleteAccount: function (id, callback) {
        return db.query("DELETE FROM accounts WHERE idaccount=?", [id], callback);

}
};

module.exports = accounts;
