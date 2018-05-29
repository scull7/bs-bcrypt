[@bs.module "bcryptjs"] external genSaltSync : (int, int) => string = "";

[@bs.module "bcryptjs"] external getRounds : string => int = "";

[@bs.module "bcryptjs"] external getHash : string => int = "";

[@bs.module "bcryptjs"]
external hashSync :
  (string, [@bs.unwrap] [ | `Length(int) | `Salt(string)]) => string =
  "";

[@bs.module "bcryptjs"] external compareSync : (string, int) => bool = "";

[@bs.module "bcryptjs"]
external genSalt : (int, (Js.Nullable.t(Js.Exn.t), string) => unit) => unit =
  "";

[@bs.module "bcryptjs"]
external hash :
  (
    string,
    [@bs.unwrap] [ | `Length(int) | `Salt(string)],
    (Js.Nullable.t(Js.Exn.t), string) => unit
  ) =>
  unit =
  "";

[@bs.module "bcryptjs"]
external compare :
  (string, string, (Js.Nullable.t(Js.Exn.t), bool) => unit) => unit =
  "";

let deNodify = (callback, err, result) =>
  switch (Js.Nullable.toOption(err)) {
  | Some(e) => callback(Belt.Result.Error(e))
  | None => callback(Belt.Result.Ok(result))
  };

let genSalt = (input, callback) => genSalt(input, deNodify(callback));

let hash = (input, salt, callback) => hash(input, salt, deNodify(callback));

let compare = (input, hash, callback) =>
  compare(input, hash, deNodify(callback));

module Promise = {
  [@bs.module "bcryptjs"]
  external genSalt : (int, int) => Js.Promise.t(string) = "";
  [@bs.module "bcryptjs"]
  external hash :
    (string, [@bs.unwrap] [ | `Length(int) | `Salt(string)]) =>
    Js.Promise.t(string) =
    "";
  [@bs.module "bcryptjs"]
  external compare : (string, string) => Js.Promise.t(bool) = "";
};
