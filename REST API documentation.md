# Sticky-notes REST API documentation

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

The business requierments that we need to met are: 

> The` user must be able to
> - create notes with title, text and tags .
> - edit notes
> - delete notes
> - search for Notes: by title / by text / by tags

## REST API

| Method | Endpoint                  | description                              | status codes |
| ------ | ------                    | ------                                   | ------       |
| GET    | /notes                    | Lists all notes created by the user.     | 204, 400, 200 |
| GET    | /notes/{noteId}           | List information for a note with 'noteId'| 204, 400, 200 |
| GET    | /notes/tags               | Lists all tags created by the user and the asscoicated notes {noteId: title} with them. | 204, 400, 200 |
| GET    | _TODO_ /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 Use requestBody for `searcByText` - too long. OR JUST /notes and request body?? | Search for notes: _by title_ and/or _by text_ and/or _by tags_ | 400, 404, 200 |
| POST   | /notes                    | Add a collection of new notes. Request in body as json. | 201 |
| PUT    | /notes                    | Update/Edit existing notes. Request in body as json | 204, 400 |
| POST   | /notes/tags               | Add new tags. Request body. | 400, 201 |
| PUT    | /notes/tags/:tagId/:tagName | Edit existing tag's name with 'tagId' to 'tagName'  | _todo_ |
| DELETE | /notes/:noteId | Delete existing note with 'noteId' | _todo_ |
| DELETE | /notes/tags/:tagId | Delete existing tag with 'tagId' | _todo_ |

Questions:
Maybe add method to delete all notes/tags with one call.

## REST API VERSION 2 LAYOUT


### Request

```console
GET localhsot:9066/api/v0/notes
```
Lists all notes created by the user.

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br> Returns Array of notes of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
GET localhsot:9066/api/v0/notes/{noteId}
```
List information for a note.
Requiers path parameters
_String `noteId` - Identifiers that notes must have to match the filter._

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br> Returns string of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
GET localhsot:9066/api/v0/notes/tags
```
Lists all tags created by the user and the asscoicated notes with them.


### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br> Returns Array of tags of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

--- 

### Request

```console
GET /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 OR JUST /notes and request body??
```
Search for notes: _by title_ and/or _by text_ and/or _by tagName
_TODO: QUESTION_ Use query params OR Request body? Should I use both `searchByTagName` and `searchByTagId`  or create `GET .../notes/tags/{tagsId} 


### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br> Returns Array of notes of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

--- 

### Request

```console
POST localhsot:9066/api/v0/notes
```
Add a collection of new notes.
_TODO Add Request body_

### Reponse
204 No Content <br>
400 Bad Request <br>
201 Created <br> Returns Identifiers of the created notes of type text/json. <br>

```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
PUT localhsot:9066/api/v0/notes
```
Update/Edit existing note.
_TODO Add Request body_

### Reponse
204 No Content - Operation does not return any data structure. Updated successfully. <br> 
400 Bad Request <br>

```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
POST localhsot:9066/api/v0/notes/tags
```
Add new tags.
_TODO Add Request body_

### Reponse
400 Bad Request <br>
201 Created <br> Returns identifiers of the created tags of type text/json. <br>

```json
TODO PASTE JSON RESPONSE
```

---