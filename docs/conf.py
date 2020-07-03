# -- Read the docs integration -----------------------------------------------
import subprocess
import os


def configureDoxyfile(_in, out):
    with open('Doxyfile.in', 'r') as file:
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INCLUDE_HEADERS@', _in)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIRECTORY@', out)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)


# Check if we're running on Read the Docs' servers
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}

if read_the_docs_build:
    input_dir = '../include'
    output_dir = '_build/html'
    configureDoxyfile(input_dir, output_dir)
    subprocess.call(['doxygen', 'Doxyfile'], shell=True, cwd='_build/html')
    breathe_projects['MikroTikApi'] = output_dir + '/xml'


# -- Project information -----------------------------------------------------
project = 'MikroTikApi'
copyright = '2020, bodand'
author = 'bodand'

release = 'v1.0.1'

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx.ext.todo',
    'sphinx.ext.githubpages',
    'breathe'
]
breathe_default_project = "MikroTikApi"
highlight_language = "cpp"
pygments_style = "perldoc"

exclude_patterns = ['Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_static_path = ['static']
html_css_files = ['ibm-font.css']
html_theme_options = {
    'navigation_depth': 6,
}
