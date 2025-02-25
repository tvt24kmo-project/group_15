const db = require('../database');

const accounts = {
    // Lisää uusi tili tietokantaan
    addAccount: function (account, callback) {
        return db.query(
            "INSERT INTO accounts (customer_id, balance, accountiban, credit_limit) VALUES (?, ?, ?, ?)", 
            [account.customer_id, account.balance, account.accountiban, account.credit_limit], 
            callback
        );
    },
    // Listaa tilien tiedot
    getAllAccounts: function (callback) {
        return db.query("SELECT * FROM accounts", callback);
    },
    // Listaa tilin tiedot kortinnumeron perusteella
    getById: function (un, callback) {
        return db.query(`SELECT 
            a.idaccount, 
            a.customer_id, 
            a.balance, 
            a.accountiban, 
            a.credit_limit
        FROM 
            accounts a
        JOIN 
            account_cards ac
        ON 
            a.idaccount = ac.account_id
        JOIN 
            cards c
        ON 
            ac.card_id = c.idcard
        WHERE 
            c.cardnumber = ?;`, [un], callback);
    },
    // Päivitä tili id:n perusteella
    updateAccount: function (account, id, callback) {
        return db.query(
            "UPDATE accounts SET customer_id=?, balance=?, accountiban=?, credit_limit=? WHERE idaccount=?", 
            [account.customer_id, account.balance, account.accountiban, account.credit_limit, id], 
            callback
        );
    },
    // Poista tili id:n perusteella
    deleteAccount: function (id, callback) {
        return db.query("DELETE FROM accounts WHERE idaccount=?", [id], callback);
    },

    
};

module.exports = accounts;

