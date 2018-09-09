import sublime
import os.path

def parseECCSources(srcList, outFlags):
    for src in srcList:
        file = src.get('file')
        if not file:
            continue

def parseECCCommon(settings, key, project, outFlags):
    flags = settings.get(key, None)
    if not flags: return

    for flag in flags:
        if project:
            flag = flag.replace('$project_base_path', project)
        outFlags.append(flag)

def parseECCParents(settings, pfx, file_name, outFlags):
    # clangd doesn't like the -i flag, and seems unnecessary for it
    return
    def addIncludePath(key, path):
        incl = settings.get(key, None)
        if not incl: return
        if isinstance(incl, bool): incl = '-I'
        outFlags.append(incl + path)
    file_name = os.path.dirname(file_name)
    addIncludePath(pfx + 'include_file_folder', file_name)
    addIncludePath(pfx + 'include_file_parent_folder', os.path.dirname(file_name))

def parseClangCompleteForDir(parent, outFlags):
    ccfile = os.path.join(parent, '.clang_complete')
    if os.path.isfile(ccfile):
        with open(ccfile, 'r', encoding='utf-8') as f:
            outFlags.extend([line.rstrip() for line in f])
        return True
    return False

def parseClangCompletes(forFile, projectPath, outFlags):
    parent = os.path.dirname(forFile)
    while parent != projectPath:
        forFile = parent
        if parseClangCompleteForDir(parent, outFlags):
            pass
        newParent = os.path.dirname(parent)
        if ( newParent == parent or not projectPath
             or not parent.startswith(projectPath) ):
            return


def getCompileFlags(view, session, file_name):
    outFlags = []
    window = view.window()
    project = window.project_file_name()
    if project:
        #parseClangCompletes(project, os.path.dirname(project), outFlags)
        project = os.path.dirname(project)
        parseClangCompleteForDir(project, outFlags)

    settings = sublime.load_settings('EasyClangComplete.sublime-settings')
    if settings:
        parseECCCommon(settings, 'common_flags', project, outFlags)
        parseECCParents(settings, '', file_name, outFlags)
    settings = window.project_data()
    if settings:
        settings = settings.get('settings')
        if settings:
            prefixes = ['ecc_', 'easy_clang_complete_']
            for pfx in prefixes:
                parseECCCommon(settings, pfx + 'common_flags', project, outFlags)
                parseECCParents(settings, pfx, file_name, outFlags)

    return outFlags

    for key in ['ecc_flags_sources', 'flags_sources']:
        srcList = view.settings().get(key)
        if (srcList):
            parseECCSources(srcList, outFlags)
