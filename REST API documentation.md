# Sticky-notes REST API documentation

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

The business requierments that we need to met are: 

> The` user must be able to
> - create notes with title, text and tags .
> - edit notes
> - delete notes
> - search for Notes: by title / by text / by tags

## REST API

| Method | Endpoint         | description| status codes |
| ------ | ------           | ------     | ------            |
| GET    | /notes           | Lists all notes created by the user.| 400 Bad Request<br> 404 Not Found<br> 200 OK returns string of type text/json |
| GET    | /notes/:noteId   | List information for a note with 'noteId'| _todo_ |
| GET    | /notes/tags      | Lists all tags created by the user and the asscoicated notes {noteId: title} with them. | _todo_ |
| GET    | _todo_ /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 searcByText maybe be long. Put In URL or send in body as json? | Search for notes: _by title_ and/or _by text_ and/or _by tags_ | 400 Bad Request<br> 404 Not Found<br> 200 OK returns string of type text/json  |
| POST   | /notes  _todo_   | Add a collection of new notes. Request in body as json. | 200 OK returns _todo_ |
| PUT    | /notes/:noteId   | Update/Edit existing note. Request in body as json | _todo_ |
| POST   | /notes/tags      | Add new tags. Request { tag1 : noteID}, {tag2 : "" }| todo |
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
_Lists all notes created by the user._

### Reponse
204 No Content
400 Bad Request
200 OK
Returns string of type text/json.