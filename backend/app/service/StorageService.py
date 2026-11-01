from pathlib import Path
from uuid import uuid4

STORAGE_ROOT = Path("storage")


def save_layer(project_id: int, layer_id: int, data: bytes) -> str:
    directory = (
        STORAGE_ROOT
        / "projects"
        / str(project_id)
        / "layers"
    )

    directory.mkdir(parents=True, exist_ok=True)

    file_id = uuid4().hex

    storage_key = (
        Path("projects")
        / str(project_id)
        / "layers"
        / f"{layer_id}_{file_id}.png"
    )

    file_path = STORAGE_ROOT / storage_key

    with file_path.open("wb") as file:
        file.write(data)

    return str(storage_key)

def get_layer(storage_key: str) -> bytes:
    data = (STORAGE_ROOT/storage_key).read_bytes()

    return data

def delete_layer(storage_key: str):
    file_path = STORAGE_ROOT / storage_key

    if file_path.exists():
        file_path.unlink()