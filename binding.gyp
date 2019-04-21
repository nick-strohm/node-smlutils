{
    'targets': [
        {
            'include_dirs': [
                "<!(node -e \"require('nan')\")"
            ],
            'target_name': 'smlutils',
            'sources': [ 'SmlUtils.cc' ],
            'win_delay_load_hook': 'true'
        }
    ]
}