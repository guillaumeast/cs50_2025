-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1. Start database
sqlite3 fiftyville.db

-- 2. Explore available tables
.tables
/*
crime_scene_reports
interviews
bakery_security_logs
atm_transactions
bank_accounts
phone_calls
airports
flights
passengers
people
*/

-- 3. Check crime_scene_report
-- 3.1. Explore schema
.schema crime_scene_reports
-- 3.2. Explore table
SELECT description FROM crime_scene_reports
 WHERE year = 2024 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
/*
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                                                                                                       description                                                                                                        |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/

-- 4. Read interviews
-- 4.1. Explore schema
.schema interviews
-- 4.2. Explore table
SELECT name, transcript FROM interviews
 WHERE year = 2024 AND month = 7 AND day = 28;
-- 4.3. Too much results -> filter with 'bakery' keyword as mentionned in crime scene report
SELECT name, transcript FROM interviews
 WHERE year = 2024 AND month = 7 AND day = 28
   AND transcript LIKE "%bakery%";
/*
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/

-- 5. Exploit Ruth interview: Search for cars that left bakery between 10:15 and 10:25
-- 5.1. Explore schema
.schema bakery_security_logs
-- 5.2. Discover activity possible values
SELECT DISTINCT activity FROM bakery_security_logs;
-- 5.3. Explore table
SELECT license_plate FROM bakery_security_logs
 WHERE year = 2024 AND month = 7 AND day = 28
   AND hour = 10 AND minute >= 15 AND minute <= 25
   AND activity = 'exit';
/*
+---------------+
| license_plate |
+---------------+
| 5P2BI95       |
| 94KL13X       |
| 6P58WS2       |
| 4328GD8       |
| G412CB7       |
| L93JTIZ       |
| 322W7JE       |
| 0NTHK55       |
+---------------+
*/

-- 6. Exploit Eugene interview: Search for withdrawals done at Leggett Street on 07/28/2024
-- 6.1. Explore schemas
.schema atm_transactions
.schema bank_accounts
.schema people
-- 6.2. Discover transaction_type possible values
SELECT DISTINCT transaction_type FROM atm_transactions;
-- 6.3. Explore table
SELECT DISTINCT p.id
  FROM people           AS p
  JOIN bank_accounts    AS b ON b.person_id = p.id
  JOIN atm_transactions AS t ON t.account_number = b.account_number
 WHERE t.year = 2024 AND t.month = 7 AND t.day = 28
   AND t.atm_location = 'Leggett Street'
   AND t.transaction_type = 'withdraw';
/*
+--------+
|   id   |
+--------+
| 395717 |
| 396669 |
| 438727 |
| 449774 |
| 458378 |
| 467400 |
| 514354 |
| 686048 |
+--------+
*/

-- 7. Exploit Raymond interview: Search for a phone call around 10:15 during less than a minute
-- 7.1. Explore schema
.schema phone_calls
-- 7.2. Explore table (guessing duration is written in seconds)
SELECT caller FROM phone_calls
 WHERE year = 2024 AND month = 7 AND day = 28
   AND duration <= 60;
/*
+----------------+
|     caller     |
+----------------+
| (130) 555-0289 |
| (499) 555-9472 |
| (367) 555-5533 |
| (609) 555-5876 |
| (499) 555-9472 |
| (286) 555-6063 |
| (770) 555-1861 |
| (031) 555-6622 |
| (826) 555-1652 |
| (338) 555-6650 |
+----------------+
*/

-- 8. Exploit Raymond interview: Search for the earliest flight out of Fiftyville tomorrow
-- 8.1. Explore schemas
.schema airports
.schema flights
.schema passengers
-- 8.2. Explore tables
SELECT passport_number FROM passengers
 WHERE flight_id = (
	  SELECT id FROM flights
	   WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
	     AND year = 2024 AND month = 7 AND day = 29
	ORDER BY hour, minute
	   LIMIT 1
 );
/*
+-----------------+
| passport_number |
+-----------------+
| 7214083635      |
| 1695452385      |
| 5773159633      |
| 1540955065      |
| 8294398571      |
| 1988161715      |
| 9878712108      |
| 8496433585      |
+-----------------+
*/

-- 9. Final crosscheck to find who the thieve is
SELECT name, phone_number, license_plate, passport_number FROM people
 WHERE license_plate IN (
        SELECT license_plate FROM bakery_security_logs
         WHERE year = 2024 AND month = 7 AND day = 28
           AND hour = 10 AND minute >= 15 AND minute <= 25
           AND activity = 'exit')
   AND id IN (
        SELECT DISTINCT p.id
          FROM people           AS p
          JOIN bank_accounts    AS b ON b.person_id = p.id
          JOIN atm_transactions AS t ON t.account_number = b.account_number
         WHERE t.year = 2024 AND t.month = 7 AND t.day = 28
           AND t.atm_location = 'Leggett Street'
           AND t.transaction_type = 'withdraw')
   AND phone_number IN (
        SELECT caller FROM phone_calls
         WHERE year = 2024 AND month = 7 AND day = 28
           AND duration <= 60)
   AND passport_number IN (
        SELECT passport_number FROM passengers
        WHERE flight_id = (
                  SELECT id FROM flights
                   WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
                     AND year = 2024 AND month = 7 AND day = 29
                  ORDER BY hour, minute
                   LIMIT 1));
/*
+-------+----------------+---------------+-----------------+
| name  |  phone_number  | license_plate | passport_number |
+-------+----------------+---------------+-----------------+
| Bruce | (367) 555-5533 | 94KL13X       | 5773159633      |
+-------+----------------+---------------+-----------------+
*/

-- 10. Final check to see where the thieve escaped to
  SELECT dest.city
    FROM airports AS dest
    JOIN flights  AS f ON destination_airport_id = dest.id
    JOIN airports AS o ON origin_airport_id = o.id
   WHERE o.city = 'Fiftyville'
     AND f.year = 2024 AND f.month = 7 AND f.day = 29
ORDER BY f.hour, f.minute
   LIMIT 1;
/*
+---------------+
|     city      |
+---------------+
| New York City |
+---------------+
*/

-- 11. Final check to see who the accomplice is
SELECT receivers.name
  FROM people      AS callers
  JOIN phone_calls AS calls ON calls.caller = callers.phone_number
  JOIN people      AS receivers ON calls.receiver = receivers.phone_number
 WHERE year = 2024 AND month = 7 AND day = 28
   AND duration <= 60
   AND calls.caller = '(367) 555-5533';
/*
+-------+
| name  |
+-------+
| Robin |
+-------+
*/
