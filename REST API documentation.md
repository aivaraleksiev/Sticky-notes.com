# Sticky-notes REST API documentation

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

The business requierments that we need to met are: 

> The` user must be able to
> - create notes with title, text and tags .
> - edit notes
> - delete notes
> - search for Notes: by title / by text / by tags

## REST API

| Method | Endpoint                  | description  |                             
| ------ | ------                    | ------       |                               
| GET    | /notes                    | Lists all notes created by the user. |     
| GET    | /notes/{noteId}           | List information for a note with 'noteId'. |
| GET    | /notes/tags               | Lists all tags created by the user and the asscoicated notes {noteId: title} with them. |
| GET _TODO_   | _TODO_ /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 Use requestBody for `searcByText` - too long. OR JUST /notes and request body?? | Search for notes: _by title_ and/or _by text_ and/or _by tags_ |
| POST   | /notes                    | Add a collection of new notes. Request in body as json. |
| PUT    | /notes                    | Update/Edit existing notes. Request in body as json.  |
| POST   | /notes/tags               | Add new tags. Request body. |
| PUT    | /notes/tags/{tagId}/{tagName} | Edit existing tag's name with 'tagId' to 'tagName' |
| DELETE | /notes/{noteId} | Delete existing note with 'noteId' |
| DELETE | /notes/tags/{tagId} | Delete existing tag with 'tagId' |

Questions:
Maybe add method to delete all notes/tags with one call.

## REST API another layout


### Request

```console
GET localhsot:9066/api/v0/notes
```
Lists all notes created by the user.

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of notes of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
GET localhsot:9066/api/v0/notes/{noteId}
```
List information for a note. <br>
Path parameters <br>
_String `noteId` - Identifiers that notes must have to match the filter._

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns string of type text/json. <br>
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
200 OK <br>
Returns Array of tags of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

--- 

### Request

```console
GET /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 OR JUST /notes and request body??
```
Search for notes: _by title_ and/or _by text_ and/or _by tagName <br>
_TODO: QUESTION_ Use query params OR Request body? Should I use both `searchByTagName` and `searchByTagId`  or create `GET .../notes/tags/{tagsId} 


### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of notes of type text/json. <br>
```json
TODO PASTE JSON RESPONSE
```

--- 

### Request

```console
POST localhsot:9066/api/v0/notes
```
Add a collection of new notes. <br>
_TODO Add Request body_ <br>

### Reponse
204 No Content <br>
400 Bad Request <br>
201 Created <br>
Returns identifiers of the created notes of type text/json. <br>

```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
PUT localhsot:9066/api/v0/notes
```
Update/Edit existing note. <br>
_TODO Add Request body_ <br>

### Reponse
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 
400 Bad Request <br>

```json
TODO PASTE JSON RESPONSE
```

---

### Request

```console
POST localhsot:9066/api/v0/notes/tags
```
Add new tags. <br>
_TODO Add Request body_ <br>

### Reponse
400 Bad Request <br>
201 Created <br>
Returns identifiers of the created tags of type text/json. <br>

```json
TODO PASTE JSON RESPONSE
```

---