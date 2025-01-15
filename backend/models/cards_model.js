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
                return db.query("INSERT INTO cards(cardtype, pinhash, islocked, credit_limit, cardnumber) values(?,?,?,?,?)",
                [cards_data.cardtype, hashed_pinhash, cards_data.islocked, cards_data.credit_limit,
                 cards_data.cardnumber], callback);
            })
        },
    update: function (id, cards_data, callback) {
        bcrypt.hash(cards_data.pinhash, 10, function (err, hashed_pinhash) {
        return db.query("UPDATE cards SET cardtype=?, pinhash=?, islocked=?, credit_limit=? WHERE idcard=?",
            [cards_data.cardtype, cards_data.pinhash, cards_data.islocked, cards_data.credit_limit, id], callback);
        })
    }
}

module.exports = cards;