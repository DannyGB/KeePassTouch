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
    signal settingsChanged()
    property var db: null
    property bool settingsLoaded: false
    property var values: {
        "theme": "Ubuntu.Components.Themes.Ambiance",
        "timeout": "60000",
        "timeoutEnabled": "1",
        "lastOpenDbName": "",
        "lastOpenDbFilePath": ""
    }
    property var dark: "Ubuntu.Components.Themes.SuruDark"
    property var light: "Ubuntu.Components.Themes.Ambiance"

    function openDB() {
        if(db !== null) return;

        db = LocalStorage.openDatabaseSync(appTitle, "0.1", "KeepIt Password Safe", 100000);

        try {
            db.transaction(function(tx) {
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
        values[key] = value
        saveSettings(values)
    }

    function getSetting(key) {
        console.debug("getSetting:" + key)
        if (settingsLoaded) {
            console.debug(values[key])
            return values[key]
        }

        getSettings()

        console.debug(values[key])

        return values[key]
    }

    function getSettings() {

        if (settingsLoaded) {
            console.debug("getSettings (cached): " + JSON.stringify(values))
            return values
        }

        openDB();
        db.transaction(function(tx) {
            var rs = tx.executeSql('SELECT key, value FROM settings;');
            
            for (var i=0, len=rs.rows.length; i<len; i++) {
                if(rs.rows.item(i).key !== null) {
                    values[rs.rows.item(i).key] = rs.rows.item(i).value
                }
            }

            settingsLoaded = true
        });        

        console.debug("getSettings: " + JSON.stringify(values))

        return values;
    }

    function saveSettings(updatedValues) {
        console.debug("saveSettings: " + JSON.stringify(updatedValues))

        try {
            openDB();
            db.transaction( function(tx){
                for (var prop in updatedValues) {
                    if(prop != null && updatedValues[prop] != null) {
                        console.debug("saveSettings: " + prop + " " + updatedValues[prop])
                        tx.executeSql('INSERT OR REPLACE INTO settings VALUES(?, ?)', [prop, updatedValues[prop]]);    
                    }
                }
            });
            settingsLoaded = false
            settingsChanged()
        }
        catch (e) {
            console.error("Error: " + JSON.stringify(e))
        }
    }
}