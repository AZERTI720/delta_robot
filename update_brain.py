import os
import subprocess
import datetime

def get_git_diff():
    """Récupère les changements du dernier commit."""
    try:
        diff = subprocess.check_output(['git', 'diff', 'HEAD~1', 'HEAD', '--name-only']).decode('utf-8')
        return diff.splitlines()
    except:
        return []

def update_journal(changes):
    """Met à jour le journal de bord Obsidian."""
    journal_path = "Brain/03_Journal/Journal_de_Bord.md"
    date_str = datetime.datetime.now().strftime("%d %B %Y")
    
    new_entry = f"\n## [{date_str}] - Mise à jour automatique\n"
    new_entry += "- **Fichiers modifiés :**\n"
    for f in changes:
        new_entry += f"  - `{f}`\n"
    
    if os.path.exists(journal_path):
        with open(journal_path, 'r') as f:
            content = f.read()
        
        # Insérer après le titre principal
        parts = content.split('\n', 1)
        updated_content = parts[0] + "\n" + new_entry + parts[1]
        
        with open(journal_path, 'w') as f:
            f.write(updated_content)

if __name__ == "__main__":
    changes = get_git_diff()
    if changes:
        update_journal(changes)
        print(f"Cerveau mis à jour avec {len(changes)} changements.")
    else:
        print("Aucun changement détecté.")
