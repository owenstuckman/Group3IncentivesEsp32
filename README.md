# Group3IncentivesEsp32

C++ with ESP32 that connects to Wi-Fi and sends posts to Supabase.

Scans QR codes with an attached camera.

Also responsible for displaying to an 16x2 LCD screen information upon scan. 

---




# Overall Software Design
```mermaid
flowchart TD

    A(Supabase - PostgreSQL) --> | Displays| B(Flutter Application)
    C(ESP32 Scanner Units) --> |Transmits Scanned Itemes|A
    B --> D(Authentication)
    D --> A 

```

# Scanner Unit Process
```mermaid
flowchart TD

    A(Start ESP32 Unit) --> B(Begin Camera Scanning)
    B --> C(HokieP Scanned)
    C --> D(Timeout Window)
    D --> B
    D--> E(Scanned an Item)
    E --> F(Scanned Item Delay - Longer)
    E --> |Post - push data|G(Supbase)
    F --> E
    F --> B
    F --> |Get - Display|G
    

```
# ERD

![image](https://github.com/user-attachments/assets/94af8931-4a54-44d4-bbbf-bbde375e9825)
