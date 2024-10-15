![AdndTK CLI](../images/charsheet.png)

AdndTK CLI - Command line tool for Advanced Dungeons & Dragons (2nd Edition)
=====================================================================

Written by Marco Carosio [https://marcocarosio.it/](https://marcocarosio.it/)

> DISCLAIMER: This software is unofficial content provided under the [Fan Content Policy](https://company.wizards.com/it/fancontentpolicy) of [Wizards of the Coast](https://company.wizards.com/)/[Hasbro](https://products.hasbro.com/it-it) and is not approved or endorsed by the company.

AdndTK CLI is released under [MIT License](LICENSE.md) and in accordance to Fan Content Policy
* it's free of charge;
* it's not an official WOTC product;
* it doesn't make use of copyrighted material;
* it's not sponsored by third parties.

AdndTK CLI is a command line tool designed to exploit the [library](../README.md) features for common tasks like characters generation and browsing the library knowledge base. It may come to handy during a game or to prepare a campaign.

## Getting Started
The CLI tool is built together with the library and tests are performed automatically to ensure the correct tool behaiour. Some common use cases are the following.

### Character generartion
The tool guides the user in a step by step character creation. The task can be achieved in different ways.

Step by step from skills generation:
<pre>
$ adndtk-cli -g [standard|best4|best_twice]
</pre>
Skills can be randomly generated according the three available procedures:
* standard: 3d6 in sequence, scores are assigned in standard order (strength, dexterity, constitution, intelligence, wisdom and charisma);
* best4: 4d6 are rolled, the least result is descarded, then scores are assigned in standard order (strength, dexterity, constitution, intelligence, wisdom and charisma);
* best_twice: 3d6 are rolled twice for each skills, the best score is kept and assigned in standard order (strength, dexterity, constitution, intelligence, wisdom and charisma).
Then the user is proposed a list of available races, according to the skills requisites. Once a race is chosen, a list of suitable classes and moral alignments are displayed.

Have fun and long life to AD&D!