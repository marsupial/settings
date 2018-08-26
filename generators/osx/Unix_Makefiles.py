from CMakeBuilder.generators import CMakeGenerator
import subprocess
import sublime
import multiprocessing


class Unix_Makefiles(CMakeGenerator):

    def __repr__(self):
        return 'Unix Makefiles'

    def file_regex(self):
        return r'(.+[^:]):(\d+):(\d+): (?:fatal )?((?:error|warning): .+)$'

    def syntax(self):
        return 'Packages/CMakeBuilder/Syntax/Make.sublime-syntax'

    def shell_cmd(self):
        return 'make -j{}'.format(str(multiprocessing.cpu_count()))

    def variants(self):
        shell_cmd = 'cmake --build . --target help'
        proc = subprocess.Popen(
            ['/bin/bash', '-l', '-c', shell_cmd],
            env=self.get_env(),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            shell=False,
            cwd=self.build_folder)
        outs, errs = proc.communicate()
        errs = errs.decode('utf-8')
        if errs:
            sublime.error_message(errs)
            return
        lines = outs.decode('utf-8').splitlines()

        variants = []
        EXCLUDES = [
            'are some of the valid targets for this Makefile:',
            'All primary targets available:',
            'depend',
            'all (the default if no target is provided)',
            'help',
            'edit_cache',
            '.ninja']

        for target in lines:
            try:
                if any(exclude in target for exclude in EXCLUDES):
                    continue
                target = target[4:]
                if (self.filter_targets and
                   not any(f in target for f in self.filter_targets)):
                    continue
                shell_cmd = 'make -j{} {}'.format(
                    str(multiprocessing.cpu_count()), target)
                variants.append({'name': target, 'shell_cmd': shell_cmd})
            except Exception as e:
                sublime.error_message(str(e))
                # Continue anyway; we're in a for-loop
        return variants
