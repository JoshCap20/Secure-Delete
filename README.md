# Secure-Delete

Simple deletion script for files and directories with three passes in accordance with most security standards.  

Pass 1 rewrites with 0s.  
Pass 2 rewrites with binary 1s.  
Pass 3 rewrites with random characters.   

Verification occurs after each pass.  

## Military and Security Standards Met:
- DoD 5220.22-M
- NIST SP 800-88 (Clear method)
- NCSC-TG-025 (Technically since 0's complement is 1)
- Air Force System Security Instruction 5020
- Aperiodic random overwrite/Random

## Potential Standards to Meet with Tweaks:
- BSI
- HMG Infosec Standard 5 (First write 1s then 0s)
