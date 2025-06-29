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

The project is a demonstration of my technical abilities (coding) and soft skills (communication and reflection). The project highlights my ability to create a product that exploits a security flaw in a unique way - through the emailing system and obfuscating the code using the champion array. In addition to this, the project shows in-depth understanding of a users interactions with the malware itself, through hiding the malware inside of an executable, hiding the location of the data and information files and implementing a XOR cipher to mask the contents of the file. Furthermore, it shows my knowledge of basic system security and ways to bypass this (employment of native OS API's and non-suspicious emailing system).
The project further demonstrates my soft skills through the report / reflection video. The video itself contains a digestable summary of what the project does and how it works. In addition to this, the video contains a reflection on the development of the project, my awareness of flaws throughout development and the ways I tried to combat / work around these flaws. Furthermore, the project video demonstrates video editing skills I have developed.
