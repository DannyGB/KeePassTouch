/*
* This file is part of KeePit
*
* Copyright (C) 2019 Dan Beavon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

import QtQuick 2.4
import QtQuick.LocalStorage 2.0

Item {    
    property var db: null

    function openDB() {
        if(db !== null) return;

        db = LocalStorage.openDatabaseSync(appTitle, "0.1", "KeepIt Password Safe", 100000);

        try {
            db.transaction(function(tx){
                tx.executeSql('CREATE TABLE IF NOT EXISTS settings(key TEXT UNIQUE, value TEXT)');
                var table  = tx.executeSql("SELECT * FROM settings");
                // Seed the table with default values
                if (table.rows.length == 0) {
                    tx.executeSql('INSERT INTO settings VALUES(?, ?)', ["theme", "Ubuntu.Components.Themes.SuruDark"]);
                    console.log('Settings table added');
                };
            });
        } catch (err) {
            console.log("Error creating table in database: " + err);
        };
    }

    function saveSetting(key, value) {
        openDB();
        db.transaction( function(tx){
            tx.executeSql('INSERT OR REPLACE INTO settings VALUES(?, ?)', [key, value]);
        });
    }

    function getSetting(key) {
        openDB();
        var res = "";
        db.transaction(function(tx) {
            var rs = tx.executeSql('SELECT value FROM settings WHERE key=?;', [key]);
            res = rs.rows.item(0).value;
        });
        return res;
    }
}