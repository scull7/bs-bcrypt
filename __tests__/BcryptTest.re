open Jest;

exception TestFailure;

describe("BCrypt Sync", () => {
  test("genSaltSync", () => {
    let salt = Bcrypt.genSaltSync(8, 0);
    Expect.expect(Js.String.length(salt)) |> Expect.toBe(29);
  });

  test("hashSync", () => {
    let hash = Bcrypt.hashSync("test_string", `Length(8));
    Expect.expect(Js.String.length(hash)) |> Expect.toBe(60);
  });
});

let failWith = e =>
  Js.Exn.message(e) |> Js.Option.getWithDefault("UNKNOWN") |> fail;

let handler = (finish, success, res) =>
  switch (res) {
  | Belt.Result.Error(e) => failWith(e) |> finish
  | Belt.Result.Ok(r) => success(r)
  };

describe("Bcrypt Async", () => {
  testAsync("genSalt", finish =>
    Bcrypt.genSalt(
      8,
      handler(finish, salt =>
        Js.String.length(salt) |> Expect.expect |> Expect.toBe(29) |> finish
      ),
    )
  );

  testAsync("hash", finish =>
    Bcrypt.hash(
      "test_string",
      `Length(8),
      handler(finish, hash =>
        Js.String.length(hash) |> Expect.expect |> Expect.toBe(60) |> finish
      ),
    )
  );

  testAsync("compare:pass", finish =>
    Bcrypt.hash(
      "test_string",
      `Length(8),
      handler(finish, hash =>
        Bcrypt.compare(
          "test_string",
          hash,
          handler(finish, result =>
            Expect.expect(result) |> ExpectJs.toBeTruthy |> finish
          ),
        )
      ),
    )
  );

  testAsync("compare:fail", finish =>
    Bcrypt.hash(
      "test_string",
      `Length(8),
      handler(finish, hash =>
        Bcrypt.compare(
          "bad_string",
          hash,
          handler(finish, result =>
            Expect.expect(result) |> ExpectJs.toBeFalsy |> finish
          ),
        )
      ),
    )
  );

  testAsync("compare:bad_hash", finish =>
    Bcrypt.hash(
      "test_string",
      `Length(8),
      handler(finish, _ =>
        Bcrypt.compare(
          "test_string",
          "foo",
          handler(finish, result =>
            Expect.expect(result) |> ExpectJs.toBeFalsy |> finish
          ),
        )
      ),
    )
  );

  testAsync("deNodify - fail", finish => {
    let callback = result =>
      switch (result) {
      | Belt.Result.Ok(_) => fail("should not be possible") |> finish
      | Belt.Result.Error(_) => pass |> finish
      };
    Bcrypt.deNodify(
      callback,
      Js.Nullable.return(TestFailure),
      Js.Nullable.null,
    );
  });
});

describe("BCrypt Promise", () => {
  testPromise("genSalt", () =>
    Bcrypt.Promise.genSalt(8, 0)
    |> Js.Promise.then_(salt =>
         Expect.expect(Js.String.length(salt))
         |> Expect.toBe(29)
         |> Js.Promise.resolve
       )
  );

  testPromise("hash", () =>
    Bcrypt.Promise.hash("test_string", `Length(8))
    |> Js.Promise.then_(hash =>
         Expect.expect(Js.String.length(hash))
         |> Expect.toBe(60)
         |> Js.Promise.resolve
       )
  );

  testPromise("compare:pass", () =>
    Bcrypt.Promise.hash("test_string", `Length(8))
    |> Js.Promise.then_(Bcrypt.Promise.compare("test_string"))
    |> Js.Promise.then_(result =>
         Expect.expect(result) |> ExpectJs.toBeTruthy |> Js.Promise.resolve
       )
  );

  testPromise("compare:fail", () =>
    Bcrypt.Promise.hash("test_string", `Length(8))
    |> Js.Promise.then_(Bcrypt.Promise.compare("should_fail"))
    |> Js.Promise.then_(result =>
         Expect.expect(result) |> ExpectJs.toBeFalsy |> Js.Promise.resolve
       )
  );
});
