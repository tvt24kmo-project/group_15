const db = require('../database');

const accounts = {
    // Lisää uusi tili tietokantaan
    addAccount: function (account, callback) {
        return db.query(
            "INSERT INTO accounts (idaccount, customer_id, balance, accountiban) VALUES (?, ?, ?, ?)", 
            [account.idaccount, account.customer_id, account.balance, account.accountiban], 
            callback
        );
    },
    // Listaa tilien tiedot
    getAllAccounts: function (callback) {
        return db.query("SELECT * FROM accounts", callback);
    },
    // Päivitä tili id:n perusteella
    updateAccount: function (account, id, callback) {
        return db.query(
            "UPDATE accounts SET idaccount=?, customer_id=?, balance=?, accountiban=? WHERE idaccount=?", 
            [account.idaccount, account.customer_id, account.balance, account.accountiban, id], 
            callback
        );
    },
    // Poista tili id:n perusteella
    deleteAccount: function (id, callback) {
        return db.query("DELETE FROM accounts WHERE idaccount=?", [id], callback);
    }
};

module.exports = accounts;

