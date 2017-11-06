open! Rebase;

module Body = Refetch__Body;
module Headers = Refetch__Headers;
module Mime = Refetch__Mime;

type t = {
  url: string,
  method: Headers.method,
  queryParams: list((string, string)),
  headers: list(Headers.t),
  body: option(Body.t)
};

let make = (method, url) => {
  url,
  method,
  queryParams: [],
  headers: [],
  body: None
};

let param = (key, value, request) => {
  ...request,
  queryParams: [(key, value), ...request.queryParams]
};

let header = (header, request) => {
  ...request,
  headers: [header, ...request.headers]
};

let payload = (payload, request) =>
  switch payload {
  /*
  | `Blob
  | `BufferSource
  | `FormData
  | `URLSearchParams
  | `ReadableStream
  */
  | `String content => {
    ...request, /* fetch will set Content-Type to text/plain for us */
    body: Some(content |> Fetch.BodyInit.make)
  }
 
  | `Json content => {
    ...header(`ContentType(Mime.json), request),
    body: Some(content |> Js.Json.stringify
                       |> Fetch.BodyInit.make)
  }
/*
  | `Dict body => 
  */
  };

let _encodeMethod =
  Fetch.(
    fun | `GET => Get
        | `HEAD => Head
        | `POST => Post
        | `PUT => Put
        | `DELETE => Delete
        | `CONNECT => Connect
        | `OPTIONS => Options
        | `TRACE => Trace
        | `PATCH => Patch
        | `OtherMethod(string) => Other(string)
  );

[@bs.val] external encodeURIComponent : string => string = "";

let _buildUrl = (url, params) => {
  let encodeParam = ((key, value)) =>
    encodeURIComponent(key) ++ "=" ++ encodeURIComponent(value);

  let joinParams =
    fun | [] => ""
        | [first, ...rest] =>
          List.reduce((acc, param) => {j|$acc&$param|j}, first, rest);

  let params =
      params |> List.map(encodeParam)
             |> joinParams;
  
  {j|$url?$params|j}
};

let _toFetchRequest = (request) =>
  Fetch.Request.makeWithInit(
    _buildUrl(request.url, request.queryParams),
    Fetch.RequestInit.make(
      ~method_=_encodeMethod(request.method),
      ~body=?request.body,
      ~headers=Headers._encode(request.headers),
    ())
  );

