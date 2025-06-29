# HORIZON-FOCUS
This is a demonstration of a windows-based keylogger written in C.

NOTE: This project is intended for review purposes ONLY. Additionally, this project will not perform as intended due to a multitude of important files missing.

# Project Description

This project was constructed for a university course. The course itself instructed students to "do something interesting security related", with a sizeable amount of wiggle room for desired projects. Students could design, teach, or learn and were required to document their findings in a report. 
My desired project was a windows-based keylogger, obfuscated within an executable file. Executable contents didn't matter, as this was all for educational purposes, and simply used as a placeholder for an enticing executable a user might wish to run. When fully packaged as intended, users would recieve a ZIP file containing their desired executable. Once run, both their executable and the keylogger malware would be run simultaneously.

# Code Execution

The fully packaged project contains a multitude of elements which work together in order to uphold an illusion of user satisfaction. A desired executable is run by the user. This is in fact a script that runs both the embedded keylogger, as well as the desired executable. This additional feature makes the malware easy to adapt to other executable files, meaning a wide range of user-desired programs can be "hijacked" to become malicious, as long as they themselves are also executable files. The majority of the project explanation (as well as a reflection on my development of the project as a whole) can be found in the attached video.

https://youtu.be/u_iNogQ4kBU

# Project Justification

