const db = require('../database');

const customers = {
    getAllCustomers: function (callback) { // Hakee kaikki asiakkaat
        return db.query("SELECT * FROM customers", callback);
    },

    addCustomer: function (customer, callback) { // Lisää uusi asiakas
        const { fname, lname, address, city, postalcode } = customer;
        return db.query("INSERT INTO customers (fname, lname, address, city, postalcode) VALUES (?, ?, ?, ?, ?)", 
        [fname, lname, address, city, postalcode], callback);
    },

    updateCustomer: function (id, customer, callback) { // Päivitä asiakas ID:n perusteella
        const { fname, lname, address, city, postalcode } = customer;
        return db.query("UPDATE customers SET fname=?, lname=?, address=?, city=?, postalcode=? WHERE idcustomer=?",
        [fname, lname, address, city, postalcode, id], callback);
    },

    deleteCustomer: function (id, callback) { // Poista asiakas ID:n perusteella
        return db.query("DELETE FROM customers WHERE idcustomer=?", [id], callback);
    },

    getFullName: function (id, callback) {
        return db.query("SELECT fname, lname FROM customers WHERE idcustomer=?", [id], callback);
    },
};

module.exports = customers;