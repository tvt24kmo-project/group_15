# Oulun ammattikorkeakoulun ohjelmistokehityksen sovellusprojekti

Tämä projekti on Oulun ammattikorkeakoulun tietotekniikan koulutusohjelman ohjelmistokehityksen sovellusprojektikurssille IN00ED14-3003 toteutettu **pankkiautomaattisovellus**. Kurssi oli 15 opintopisteen laajuinen.

## Projektin tavoitteet

- Toteuttaa toimiva sovellus projektisuunnitelman mukaan
- Soveltaa seuraavilla kursseilla opittuja taitoja käytännössä:
    - Johdatus ohjelmointiin (5op)
    - Olio-ohjelmointi ja oliopohjainen suunnittelu (5op)
    - Tietokannat ja rajapinnat (5op)
    - Linux Administration (5op)
- Työskennellä tehokkaasti tiimissä
- Oppia projektinhallintaa ja ohjelmistokehityksen parhaita käytäntöjä

## Projektiryhmä

Projektiryhmä koostuu seuraavista henkilöistä:

<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="20%"><a href="https://github.com/mintusmaximus"><img src="https://avatars.githubusercontent.com/u/156218528?v=4" width="100px;" alt="Jesse Heikkinen"/><br /><sub><b>Jesse Heikkinen</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=mintusmaximus" title=""></a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/eemil3"><img src="https://avatars.githubusercontent.com/u/181833755?v=4" width="100px;" alt="Eemil Koskelo"/><br /><sub><b>Eemil Koskelo</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=eemil3" title=""></a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/nnksknkngs"><img src="https://avatars.githubusercontent.com/u/169275720?v=4" width="100px;" alt="Anna Koskenkangas"/><br /><sub><b>Anna Koskenkangas</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=nnksknkngs" title=""></a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/jarmoit"><img src="https://avatars.githubusercontent.com/u/181835775?v=4" width="100px;" alt="Jarmo Marjakangas"/><br /><sub><b>Jarmo Marjakangas</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=Jarmoit" title=""></a></td>
    </tr>
  </tbody>
</table>

## Ominaisuudet
- **Tietokanta:** Asiakas-, tili- ja korttitiedot tallennetaan MySQL-tietokantaan
- **REST API:** Node.js-pohjainen API mahdollistaa tietokannan ja Qt-sovelluksen välisen kommunikoinnin
- **Pankkiautomaattisovellus:** Qt-työpöytäsovellus, jossa käytetään QT Network -moduulia

## Tietokannan pyörittäminen paikallisesti:
- Kloonaa repositorio
- Avaa MySQL Workbench
- Luo uusi schema nimeltä `atm_automat`
- Importtaa MySQL dumppitiedosto valikosta `Server -> Data Import -> Import from Self-Contained File` 
- Valitse target schema jonka loit äsken
- Importtaa dumppi
- Luo `.env` tiedosto `/backend` kansioon
- Kopioi `env_template` tiedoston tiedot omaan `/backend/.env` tiedostoon
- Täytä `.env` tiedostoon omat MySQL yhteyden parametrit
- Avaa konsoli-ikkuna `/backend` kansioon
- Aja komento `npm install` ja `npm start`
- NodeJS Serveri testaa käynnistäessä yhteyttä tietokantaan

Tietokannan ER-kaavio:
<img src="img/ER_kaavio.png">

## Lisenssi
Tämä projekti on lisensoitu MIT-lisenssillä. Lisätiedot [LICENSE](LICENSE)-tiedostosta.