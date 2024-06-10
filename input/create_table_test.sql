CREATE TABLE IF NOT EXISTS "user" (
    id SERIAL,
    username VARCHAR(255) NOT NULL,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS smart_pi (
    serial_number VARCHAR(255) PRIMARY KEY,
    user_id INT,
    FOREIGN KEY (user_id) REFERENCES "user" (id)
);

CREATE TABLE IF NOT EXISTS measurement (
    smart_pi_serial_number VARCHAR(255),
    time TIMESTAMP,
    power1 FLOAT,
    power2 FLOAT,
    power3 FLOAT,
    PRIMARY KEY (smart_pi_serial_number, time),
    FOREIGN KEY (smart_pi_serial_number) REFERENCES smart_pi (serial_number)
);


