const db = require('../database');
const bcrypt = require('bcryptjs');

const cards = {
    getAllCards: function (callback) {
        return db.query("SELECT * FROM cards", callback);
    },

    getByCardnumber: function (cardnumber, callback) {
        return db.query("select * from cards where cardnumber=?", [cardnumber], callback);
    },

    //kortin salasanan kryptaus
    add: function (cards_data, callback) {
        bcrypt.hash(cards_data.pinhash, 10, function (err, hashed_pinhash) {
            return db.query("INSERT INTO cards(cardtype, pinhash, islocked, cardnumber) values(?,?,?,?)",
                [cards_data.cardtype, hashed_pinhash, cards_data.islocked, cards_data.cardnumber], callback);
        })
    },

    //kortin tietojen päivitys id:n perusteella
    update: function (id, update_cards_data, callback) {
        bcrypt.hash(update_cards_data.pinhash, 10, function (err, hashed_pinhash) {
            return db.query("UPDATE cards SET cardtype=?, pinhash=?, islocked=? WHERE idcard=?",
                [update_cards_data.cardtype, hashed_pinhash, update_cards_data.islocked, id], callback);
        })
    },

    
    //kortin poisto id:n perusteella
    delete: function (id, callback) {   
        return db.query("DELETE FROM cards WHERE idcard=?", [id], callback);
    },
    //kortin salasanan tarkistus
    checkPassword: function(cardnumber, callback) {
        return db.query("SELECT pinhash FROM cards WHERE cardnumber=?", [cardnumber], callback);
    },

    //kortin väärän yrityksen päivitys kortin numeron perusteella (sama kun loginissa)
    updateWrongAttempt: function(wrong_attempts, cardnumber, callback) {
        return db.query("UPDATE cards SET wrong_attempts=? WHERE cardnumber=?", [wrong_attempts, cardnumber], callback);
    },

    checkLockStatus: function(cardnumber, callback) {
        return db.query("SELECT islocked FROM cards WHERE cardnumber=?", [cardnumber], callback);
    },

    checkCardAttempts: function(cardnumber, callback) {
        return db.query("SELECT wrong_attempts FROM cards WHERE cardnumber=?", [cardnumber], callback);
    },

    getCustomerByCardNumber: function(cardnumber, callback) {
        return db.query(
            'SELECT c.idcustomer FROM customers c JOIN accounts a ON c.idcustomer = a.customer_id JOIN account_cards ac ON a.idaccount = ac.account_id JOIN cards ca ON ac.card_id = ca.idcard WHERE ca.cardnumber = ?',
            [cardnumber],
            callback
        );
    },
}

module.exports = cards;
