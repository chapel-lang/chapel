

module FormatHelper {
  use Json;
  use IO;

  enum FormatKind {
    default,
    json
  }

  config param format : FormatKind = FormatKind.default;

  proc getFormatType(param writing : bool) type {
    select format {
      when FormatKind.default {
        if writing then return IO.DefaultWriter;
        else return IO.DefaultReader;
      }
      when FormatKind.json {
        if writing then return JsonWriter;
        else return JsonReader;
      }
      otherwise return nothing;
    }
  }

  type FormatReader = getFormatType(false);
  type FormatWriter = getFormatType(true);
}
