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
    //kortin tietojen päivitys id:n perusteella
    update: function (id, update_cards_data, callback) {
        bcrypt.hash(cards_data.pinhash, 10, function (err, hashed_pinhash) {
            return db.query("UPDATE cards SET cardtype=?, pinhash=?, islocked=?, credit_limit=? WHERE idcard=?",
                [update_cards_data.cardtype, hashed_pinhash, update_cards_data.islocked, update_cards_data.credit_limit, id], callback);
        })
    },
    //kortin poisto id:n perusteella
    delete: function (id, callback) {
        return db.query("DELETE FROM cards WHERE idcard=?", [id], callback);
    }
}

module.exports = cards;