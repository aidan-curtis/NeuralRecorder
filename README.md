<iframe style="width: 100%" height = "500" src="https://www.youtube.com/embed/SluLUA3ErF0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


## Problem Statement
According to the World Health Organization, up to 50 million people in the world have epilepsy. Of these 50 million, 30% do not respond to medication to keep their seizures under control. These patients are said to have intractable epilepsy, and one of the most effective ways of treating these patients is to record patient EEG data in order to determine the source of the seizures, and then remove the specific part of the brain that is causing the seizures. This process of recording data is invasive and requires implanting electrodes into the brain. These electrodes are wired from inside the brain to a stationary external source that collects the data. The presence of wires causes two main problems: 1) the patients have limited mobility throughout the duration of the data recording and must remain in the hospital for days or even weeks, and 2) the wires are a potential source of infection.

Remaining in the hospital for an extended period of time is extremely expensive, and if the wires were to cause an infection, that cost would only go up. The estimated cost for this procedure is about $10,000 a day. Additionally, being unable to move about unrestricted is detrimental to the patient’s general well-being.

We aim to design a device that can be implanted into a patient’s brain and transmit EEG data wirelessly. Such a device can decrease hospital stay with the possibility of the patient going home for the duration of the recording process, and will drastically decrease the cost of the process. Additionally, even if the patient must remain in the hospital, our device will allow the patient to move about the room freely, which will greatly improve their quality of life. Lastly, our device will also implement novel data compression techniques prior to transmission, which sets our product apart from existing solutions.

## Project Scope
We will build a fully functioning wireless neural recording system that uses data compression to efficiently transmit local field potentials and/or single unit action potential data from patients to a nearby data collection device.

It will contain four major subsystems:
- A power management system
- Frontend interface chip that does signal conditioning
- A wireless link to a receiver
- A novel data compression implementation

With a budget of $2,500 and working between August 29, 2018 and April 11, 2019 we must meet or exceed a minimum set of requirements:
- 48 electrode channels consisting of 6 probes with 8 electrodes each
- A battery life 24 hours or greater
- An ADC resolution of at least 8 bits
- Ability to wirelessly transmit data at least 1 meter
- A Sampling rate of at least 1000 Hz

## Intracranial Testing
<iframe style="width: 100%" height = "500" src="https://www.youtube.com/embed/LJeJShj3aGc" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## ISCAS World Champions
We placed first in the ISCAS Compatition in Sapporo, Japan.
![ISCAS Award]( iscas_poster.jpg )
![ISCAS Poster]( iscas.jpg )

## Team
![Team Axon Mobile]( team photo.jpg )
- *Aidan Curtis:* arc11 at rice dot edu
- *Sophie D'Amico:* srd5 at rice dot edu
- *Benjamin Klimko:* bhk2 at rice dot edu
- *Irene Zhang:* zz36 at rice dot edu
- *Andres Gomez:* aeg6 at rice dot edu

## Sponsorship
Thanks to Altium for sponsoring our team and providing us with Altium Designer so we can easily and efficiently create custom PCBs for our project.
