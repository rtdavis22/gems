{
  'target_defaults': {
    'cflags': [
      '-pthread'
    ],

    'ldflags': [
      '-pthread'
    ]
  },

  'variables': {
    'target_arch': 'x64'
  },
  'targets': [
    {
      'target_name': 'gems',
      'type': 'executable',

      'dependencies': [
        '../deps/v8/tools/gyp/v8.gyp:v8'
      ],

      'include_dirs': [
        '../src',
        '../deps/gmml/include',
        '../deps/v8/include'
      ],

      'link_settings': {
        'libraries': [
           '-lpthread',
           # Hack, because it seems library_dirs below doesn't work. The
           # directory is relative the the Makefile (out/Makefile).
           '../deps/gmml/libgmml.a'
        ],
        'library_dirs': [
          'deps/gmml'
        ]
      },

      'sources': [
        '../src/atom.cc',
        '../src/environment.cc',
        '../src/gems.cc',
        '../src/glycam.cc',
        '../src/pdb.cc',
        '../src/shell.cc',
        '../src/structure.cc',
      ],

      'defines': [
      ],
    },
  ] # end targets
}

